/*
 * Footstep Power Generation - Arduino Uno monitor
 *
 * Reads the rectified/divided piezo array voltage (A0) and the battery
 * voltage (A1), counts footsteps, shows live values on a 16x2 LCD and
 * streams a CSV line over Serial (9600 baud):  ms,peak_v,bat_v,steps
 *
 * Optional: SMS battery status over a SIM800L-type GSM module
 * (set ENABLE_GSM to 1). See docs/hardware.md for wiring.
 */
#include <LiquidCrystal.h>
#include "config.h"

#if ENABLE_GSM
#include <SoftwareSerial.h>
SoftwareSerial gsm(GSM_RX_PIN, GSM_TX_PIN);
#endif

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

unsigned long steps = 0;
unsigned long lastStepMs = 0;
unsigned long lastReportMs = 0;
float windowPeakV = 0.0f;     // highest piezo voltage seen since last report
float lastStepPeakV = 0.0f;
bool  aboveThreshold = false;
uint8_t lastSmsBucket = 255;

static float adcToVolts(int raw, float dividerRatio) {
  return (raw * ADC_REF_V / 1023.0f) * dividerRatio;
}

static int batteryPercent(float v) {
  if (v <= BAT_EMPTY_V) return 0;
  if (v >= BAT_FULL_V)  return 100;
  return (int)((v - BAT_EMPTY_V) * 100.0f / (BAT_FULL_V - BAT_EMPTY_V));
}

#if ENABLE_GSM
static void sendSms(const char *text) {
  gsm.println(F("AT+CMGF=1"));
  delay(300);
  gsm.print(F("AT+CMGS=\""));
  gsm.print(GSM_PHONE_NUMBER);
  gsm.println(F("\""));
  delay(300);
  gsm.print(text);
  gsm.write(26);  // Ctrl+Z ends the message
  delay(3000);
}

// Send an SMS only when the battery crosses into a new 25% bucket.
static void maybeSendSms(int pct, float v) {
  uint8_t bucket = pct / 25;
  if (bucket == lastSmsBucket) return;
  lastSmsBucket = bucket;
  char msg[48];
  char vbuf[8];
  dtostrf(v, 4, 2, vbuf);
  snprintf(msg, sizeof(msg), "Footstep gen: battery %d%% (%sV)", pct, vbuf);
  sendSms(msg);
}
#endif

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print(F("Footstep Power"));
  lcd.setCursor(0, 1);
  lcd.print(F("Generator  v1.0"));
#if ENABLE_GSM
  gsm.begin(9600);
#endif
  Serial.println(F("ms,peak_v,bat_v,steps"));
  delay(1500);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();

  // --- sample the piezo channel and track the peak ---
  float pv = adcToVolts(analogRead(PIEZO_PIN), PIEZO_DIVIDER_RATIO);
  if (pv > windowPeakV) windowPeakV = pv;

  // --- step detection: rising edge through threshold, with debounce ---
  if (pv >= STEP_THRESHOLD_V) {
    if (!aboveThreshold && (now - lastStepMs) >= STEP_DEBOUNCE_MS) {
      steps++;
      lastStepMs = now;
      lastStepPeakV = pv;
    }
    aboveThreshold = true;
    if (pv > lastStepPeakV) lastStepPeakV = pv;
  } else if (pv < STEP_THRESHOLD_V * 0.5f) {
    aboveThreshold = false;   // hysteresis so one footfall counts once
  }

  // --- periodic display + log ---
  if (now - lastReportMs >= REPORT_INTERVAL_MS) {
    lastReportMs = now;
    float bv = adcToVolts(analogRead(BATTERY_PIN), BATTERY_DIVIDER_RATIO);
    int pct = batteryPercent(bv);

    lcd.setCursor(0, 0);
    lcd.print(F("Pk:"));
    lcd.print(windowPeakV, 1);
    lcd.print(F("V S:"));
    lcd.print(steps);
    lcd.print(F("      "));
    lcd.setCursor(0, 1);
    lcd.print(F("Bat:"));
    lcd.print(bv, 2);
    lcd.print(F("V "));
    lcd.print(pct);
    lcd.print(F("%   "));

    Serial.print(now);            Serial.print(',');
    Serial.print(windowPeakV, 2); Serial.print(',');
    Serial.print(bv, 3);          Serial.print(',');
    Serial.println(steps);

#if ENABLE_GSM
    maybeSendSms(pct, bv);
#endif
    windowPeakV = 0.0f;
  }
}
