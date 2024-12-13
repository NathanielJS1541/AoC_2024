import re
import sys
from dataclasses import dataclass
from pathlib import Path

import numpy as np


@dataclass
class ClawMachine:
    ButtonX: tuple[int, int]
    ButtonY: tuple[int, int]
    Prize: tuple[int, int]
    AddOffset: bool = False

    def ActivateOffset(self):
        self.AddOffset = True

    @property
    def Offset(self) -> int:
        return 10000000000000 if self.AddOffset else 0

    @property
    def PrizeVal(self) -> tuple[int, int]:
        p1 = self.Prize[0] + self.Offset
        p2 = self.Prize[1] + self.Offset
        return (p1, p2)

    @classmethod
    def FromStr(cls, inputStr) -> "ClawMachine":
        buttonAX = int(re.findall(r"(?<=A: X\+)\d+", inputStr)[0])
        buttonAY = int(re.findall(rf"(?<=A: X\+{buttonAX}\, Y\+)\d+", inputStr)[0])
        buttonBX = int(re.findall(r"(?<=B: X\+)\d+", inputStr)[0])
        buttonBY = int(re.findall(rf"(?<=B: X\+{buttonBX}\, Y\+)\d+", inputStr)[0])
        prizeX = int(re.findall(r"(?<=X\=)\d+", inputStr)[0])
        prizeY = int(re.findall(r"(?<=Y\=)\d+", inputStr)[0])
        return cls(
            ButtonX=(buttonAX, buttonBX),
            ButtonY=(buttonAY, buttonBY),
            Prize=(prizeX, prizeY),
        )

    @classmethod
    def ExtractList(cls, text) -> list["ClawMachine"]:
        claws: list["ClawMachine"] = []
        currentStr = ""
        for t in text.split("\n"):
            if t != "":
                currentStr += "\n" + t
            else:
                claws.append(cls.FromStr(currentStr))
                currentStr = ""
        claws.append(cls.FromStr(currentStr))
        return claws

    def SolveButtonPresses(self) -> int:
        eqs = np.array([self.ButtonX, self.ButtonY])
        results = np.array(self.PrizeVal)
        # cspell: disable-next-line
        result = np.linalg.solve(eqs, results)
        return (result[0] * 3 + result[1]) if self.CheckButton(result[0], result[1]) else 0

    def CheckButton(self, xVal, yVal):
        xVal = round(xVal)
        yVal = round(yVal)
        x = xVal * self.ButtonX[0] + yVal * self.ButtonX[1]
        y = xVal * self.ButtonY[0] + yVal * self.ButtonY[1]
        return x == self.PrizeVal[0] and y == self.PrizeVal[1]


def Main(path: Path):
    claws = ClawMachine.ExtractList(path.read_text())
    tokens = [x.SolveButtonPresses() for x in claws if x.SolveButtonPresses() > 0]
    print(sum(tokens))
    [x.ActivateOffset() for x in claws]
    tokens = [x.SolveButtonPresses() for x in claws if x.SolveButtonPresses() > 0]
    print(sum(tokens))


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
