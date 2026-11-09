import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "tools"))
from analyze_log import parse_rows, summarize  # noqa: E402

LOG = """ms,peak_v,bat_v,steps
500,0.10,3.800,0
1000,12.50,3.801,1
1500,3.20,3.803,1
garbage line
2000,20.00,3.810,3
"""


class AnalyzeLogTest(unittest.TestCase):
    def test_skips_header_and_garbage(self):
        rows = list(parse_rows(LOG.splitlines()))
        self.assertEqual(len(rows), 4)

    def test_summary(self):
        s = summarize(parse_rows(LOG.splitlines()))
        self.assertEqual(s.steps, 3)
        self.assertAlmostEqual(s.duration_s, 1.5)
        self.assertAlmostEqual(s.max_peak_v, 20.0)
        self.assertAlmostEqual(s.bat_delta_v, 0.010, places=3)
        self.assertAlmostEqual(s.steps_per_min, 120.0)

    def test_empty_raises(self):
        with self.assertRaises(ValueError):
            summarize(parse_rows(["ms,peak_v,bat_v,steps"]))


if __name__ == "__main__":
    unittest.main()
