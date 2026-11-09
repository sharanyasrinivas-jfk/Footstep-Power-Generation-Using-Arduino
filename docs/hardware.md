# Hardware Design

Architecture follows the "Circuit Design Insights" slide of the survey.

## Signal chain
Piezo array -> bridge rectifier -> smoothing capacitor -> (a) voltage regulator /
boost to 5 V -> TP4056 Li-ion charger -> battery, and (b) divider -> Arduino A0.

## Bill of materials
| Qty | Part | Note |
|-----|------|------|
| 1 | Arduino Uno | Monitoring |
| 6-12 | Piezo discs (27-35 mm) | Series-parallel array under a tile |
| 1 | Bridge rectifier | 4x 1N5819 Schottky diodes (low drop) |
| 2 | Anti-parallel protection diodes | Across the array output (Paper 2) |
| 1 | 1000 uF / 25 V capacitor | Smoothing |
| 1 | 5 V regulator / boost module | Output stage |
| 1 | TP4056 module with protection | 1S Li-ion charging |
| 1 | 18650 cell | Storage |
| 1 | 16x2 LCD (HD44780) + 10k pot | Contrast |
| 1 | 1N4733A 5.1 V zener | Clamp on A0 |
| misc | 100k, 12k, 2x10k resistors | Dividers |
| opt | SIM800L GSM module | SMS status (Paper 3) |

## Arduino wiring
| Signal | Pin |
|--------|-----|
| Piezo sense (after 100k/12k divider + 5.1 V zener) | A0 |
| Battery sense (10k/10k divider) | A1 |
| LCD RS, E | D12, D11 |
| LCD D4-D7 | D5, D4, D3, D2 |
| GSM RX/TX (optional) | D7, D8 |

Piezo divider: top 100k from rectified bus to A0, 12k from A0 to GND, zener
(cathode to A0) across the 12k. Ratio 9.33 gives ~4.8 V at 45 V.

## Safety
- Raw piezo spikes can exceed 40 V: never connect the array directly to the Arduino.
- Use a protected Li-ion module; never charge bare cells without protection.
- SIM800L needs a separate supply that can deliver ~2 A bursts and a 5 V -> 2.8 V level shift on its RX line.
