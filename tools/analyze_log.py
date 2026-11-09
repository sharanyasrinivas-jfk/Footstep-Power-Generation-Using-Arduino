#!/usr/bin/env python3
"""Summarise a CSV log captured from the Arduino serial port.

Expected columns (as printed by the firmware): ms,peak_v,bat_v,steps

Capture a log with e.g.:   python -m serial.tools.miniterm /dev/ttyACM0 9600 > run.csv
Usage:                     python tools/analyze_log.py run.csv
"""
import csv
import sys
from dataclasses import dataclass


@dataclass
class Summary:
    samples: int
    duration_s: float
    steps: int
    max_peak_v: float
    mean_peak_v: float
    bat_start_v: float
    bat_end_v: float

    @property
    def steps_per_min(self) -> float:
        return self.steps / (self.duration_s / 60.0) if self.duration_s > 0 else 0.0

    @property
    def bat_delta_v(self) -> float:
        return self.bat_end_v - self.bat_start_v


def parse_rows(lines):
    """Yield (ms, peak_v, bat_v, steps); skip headers and malformed lines."""
    for row in csv.reader(lines):
        if len(row) != 4:
            continue
        try:
            yield int(row[0]), float(row[1]), float(row[2]), int(row[3])
        except ValueError:
            continue


def summarize(rows) -> Summary:
    rows = list(rows)
    if not rows:
        raise ValueError("no valid rows in log")
    peaks = [r[1] for r in rows]
    return Summary(
        samples=len(rows),
        duration_s=(rows[-1][0] - rows[0][0]) / 1000.0,
        steps=rows[-1][3] - rows[0][3],
        max_peak_v=max(peaks),
        mean_peak_v=sum(peaks) / len(peaks),
        bat_start_v=rows[0][2],
        bat_end_v=rows[-1][2],
    )


def main(argv):
    if len(argv) != 2:
        print(__doc__)
        return 2
    with open(argv[1], newline="") as f:
        s = summarize(parse_rows(f))
    print(f"Samples        : {s.samples}")
    print(f"Duration       : {s.duration_s:.1f} s")
    print(f"Steps          : {s.steps} ({s.steps_per_min:.1f}/min)")
    print(f"Peak voltage   : max {s.max_peak_v:.2f} V, mean {s.mean_peak_v:.2f} V")
    print(f"Battery        : {s.bat_start_v:.3f} V -> {s.bat_end_v:.3f} V ({s.bat_delta_v:+.3f} V)")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
