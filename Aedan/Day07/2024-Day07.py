import itertools
import sys
from dataclasses import dataclass
from pathlib import Path

OPERANDS = [lambda x, y: x * y, lambda x, y: x + y]


@dataclass
class Calibration:
    result: int
    values: list[int]

    def TestPossible(self) -> int:
        for comb in itertools.product(OPERANDS, repeat=len(self.values) - 1):
            sumTest = self.values[0]
            for idx, func in enumerate(comb):
                sumTest = func(sumTest, self.values[idx + 1])
            if sumTest == self.result:
                return self.result
        return 0

    @classmethod
    def LoadFromStr(cls, strInput):
        split = strInput.split(":")
        r = int(split[0])
        v = [int(x) for x in split[1].split()]
        return cls(r, v)


def Main(path: Path) -> None:
    objs: list[Calibration] = []
    for x in path.read_text().split("\n"):
        objs.append(Calibration.LoadFromStr(x))
    p1Total = sum([o.TestPossible() for o in objs])
    print(p1Total)
    OPERANDS.append(lambda x, y: int(str(x) + str(y)))
    p2Total = sum([o.TestPossible() for o in objs])
    print(p2Total)


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
