#ifndef FOOTSTEP_CONFIG_H
#define FOOTSTEP_CONFIG_H

// ---- Analog inputs ----
#define PIEZO_PIN    A0
#define BATTERY_PIN  A1
#define ADC_REF_V    5.0f

// Piezo divider: 100k (top) / 12k (bottom) -> ratio (100+12)/12.
// Maps ~45 V peak (Paper 2) to ~4.8 V at the pin. A 5.1 V zener across
// the 12k resistor clamps spikes.
#define PIEZO_DIVIDER_RATIO    9.333f
// Battery divider: 10k / 10k -> ratio 2
#define BATTERY_DIVIDER_RATIO  2.0f

// ---- Step detection ----
#define STEP_THRESHOLD_V  2.0f   // tune on your tile
#define STEP_DEBOUNCE_MS  150UL

// ---- Reporting ----
#define REPORT_INTERVAL_MS 500UL

// ---- Single-cell Li-ion battery (TP4056 charger module) ----
#define BAT_EMPTY_V 3.3f
#define BAT_FULL_V  4.2f

// ---- 16x2 LCD (parallel, 4-bit) ----
#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

// ---- Optional GSM SMS status (Paper 3) ----
#define ENABLE_GSM 0
#define GSM_RX_PIN 7            // Arduino RX  <- GSM TX
#define GSM_TX_PIN 8            // Arduino TX  -> GSM RX (use a divider!)
#define GSM_PHONE_NUMBER "+910000000000"

#endif
