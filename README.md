# Footstep Power Generation Using Arduino

Piezoelectric footstep energy harvester with an Arduino Uno monitor. Based on a
literature survey of five papers (2020-2022); this repo contains the monitoring
firmware, a log-analysis tool, and design documentation.

## Repository layout
```
firmware/footstep_power_monitor/   Arduino sketch + config.h
tools/analyze_log.py               Summarise serial CSV logs
tests/                             Unit tests for the tool
docs/literature-survey.md          Survey findings
docs/hardware.md                   BOM, wiring, safety
hardware/wiring.txt                ASCII schematic
```

## What the firmware does
- Tracks the peak piezo voltage on A0 and counts footsteps (threshold + debounce + hysteresis)
- Reads battery voltage on A1, shows percent
- Displays `Pk / steps / battery` on a 16x2 LCD
- Streams `ms,peak_v,bat_v,steps` CSV at 9600 baud
- Optional SMS battery status via GSM (`ENABLE_GSM 1` in `config.h`)

## Getting started
1. Build the circuit per [docs/hardware.md](docs/hardware.md).
2. Open `firmware/footstep_power_monitor/footstep_power_monitor.ino` in the Arduino IDE
   (uses the built-in `LiquidCrystal` library), select *Arduino Uno*, upload.
3. Tune `STEP_THRESHOLD_V` in `config.h` for your tile.
4. Log and analyse:
   ```bash
   python -m serial.tools.miniterm /dev/ttyACM0 9600 > run.csv   # pip install pyserial
   python tools/analyze_log.py run.csv
   ```
5. Run tests: `python -m unittest discover tests`

## Status
Firmware is untested on hardware; thresholds and divider values need calibration.

## Team
Shraddha K.V, Tejaswini M.S, V. Soundarya, Sharanya S
Guide: J.S. Shwetha | Reviewer: Dhanusha H

## License
MIT
