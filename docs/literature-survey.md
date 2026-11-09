# Literature Survey Summary

Source: *Footstep Power Generation Using Arduino* literature survey presentation
(RNS Institute of Technology). Five papers (2020-2022) on piezoelectric footstep
generation with Arduino were reviewed.

## Principle
Piezoelectric materials (PZT crystals, ceramic discs) produce charge under
mechanical stress. Energy chain:

    Footstep pressure -> Piezo sensor (AC) -> Rectifier (DC) -> Battery -> Load

## Papers

| # | Paper | Venue / Year | Key point |
|---|-------|--------------|-----------|
| 1 | Foot-step Power Generation System | Aureas Journal, 2022 | Simple piezo setup with Arduino monitoring; powers AC and DC loads |
| 2 | Design of Sustainable Pedestrian Power Generator | Indian J. Sci. & Tech., 2020 | 45 V without protection; stable 20 V with anti-parallel diodes |
| 3 | Arduino Power Generating Using Human Foot Step | JES Publication, 2020 | GSM remote battery status via SMS; LCD voltage display |
| 4 | Power Generation Using Piezoelectric Material | IJSRST, 2020 | 9-12 V from series-parallel sensor configuration |
| 5 | Footstep Power Generation Using Piezoelectric Sensor | Sri Raaja Raajan College | 20 V (protected), voltage boosting; PIC microcontroller |

## Challenges and solutions

| Challenge | Solution |
|-----------|----------|
| Low current output | Multiple sensors in parallel |
| Voltage fluctuation | Bridge rectifier + smoothing capacitors |
| Reverse-current damage | Anti-parallel diodes (Paper 2) |
| Storage efficiency | Optimised battery charging circuit |

## Applications / future scope
LED lighting, phone charging, small DC devices, emergency lighting; schools,
offices, malls, stations; IoT monitoring, solar integration, smart street lighting.
