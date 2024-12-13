import ctypes
import pathlib
import sys
import time
from pathlib import Path

from progress.bar import Bar


class T_Data(ctypes.Structure):
    _fields_ = [
        ("val1", ctypes.c_long),
        ("val2", ctypes.c_long),
    ]

    def __repr__(self) -> str:
        return f"{self.val1}, {self.val2}"

    def __str__(self) -> str:
        return f"{self.val1}, {self.val2}"


def Main(path: Path, count: int):
    c_lib = ctypes.CDLL(__file__.replace(".py", ".so"))
    c_lib.ApplyRules.restype = ctypes.c_void_p
    rocks: list[int] = [int(x) for x in path.read_text().replace("\n", "").split() if x != ""]
    # with Bar(max=count, suffix="%(index)d/%(max)d - Elapsed:%(elapsed)ds ") as bar:
    for idx in range(count):
        start = time.time()
        result = []
        for rock in rocks:
            print(rock, idx)
            res = c_lib.ApplyRules(ctypes.c_int(rock))
            rockResult = T_Data.from_address(res)
            print(rockResult)
            result.append(int(rockResult.val1))
            if rockResult.val2 > 0:
                result.append(int(rockResult.val2))
        rocks = result
        print(idx, round(time.time() - start, 3), len(rocks))
        # bar.next()
    print(len(rocks))


if __name__ == "__main__":
    p = Path(
        r"C:\mysources\Aedan\AOC\AoC_2024\Aedan\Day11\2024-Day11-Sample.txt"
    )  # Path(sys.argv[1])
    count = 25  # int(sys.argv[2])
    Main(p, count)
