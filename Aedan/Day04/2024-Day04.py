import re
import sys
from pathlib import Path
from pprint import pp


def rotate(l, n):
    if n > 0:
        l = l[n:] + ["."] * n
    elif n < 0:
        l = ["."] * abs(n) + l[:n]
    return l


def GetHorizontal(string: str):
    forwards = len(list(re.finditer("XMAS", string)))
    backwards = len(list(re.finditer("SAMX", string)))

    return forwards + backwards


def GetVert(string: str):
    lines = string.split("\n")
    charArray = ["".join([x[idx] for x in lines]) for idx in range(len(lines[0]))]
    return GetHorizontal("\n".join(charArray))


def Diag(path: Path):
    sumVal = 0
    lines = path.read_text().split("\n")

    for startLine in range(len(lines) - 3):
        rotateSection = lines[startLine : 4 + startLine]
        charArray = ["".join(rotate(list(line), idx)) for idx, line in enumerate(rotateSection)]

        left = GetVert("\n".join(charArray))
        charArray = [
            "".join(rotate(list(line), -1 * idx)) for idx, line in enumerate(rotateSection)
        ]
        right = GetVert("\n".join(charArray))
        sumVal += left + right
    return sumVal


def Part1(path: Path):
    sumTotal = 0
    sumTotal += GetHorizontal(path.read_text())
    sumTotal += GetVert(path.read_text())
    sumTotal += Diag(path)
    print(sumTotal)


def Part2(path: Path):
    charArray = [[char for char in line] for line in path.read_text().split("\n")]
    aPos = []
    xmasCount = 0
    for lineNum, line in enumerate(charArray):
        if lineNum not in [len(charArray), 0]:
            for colNum, col in enumerate(line):
                if col == "A" and colNum not in [len(line), 0]:
                    aPos.append((lineNum, colNum))

    for pos in set(aPos):
        try:
            row, col = pos
            crossText = [
                [
                    charArray[row - 1][col - 1],
                    charArray[row + 1][col + 1],
                ],
                [
                    charArray[row - 1][col + 1],
                    charArray[row + 1][col - 1],
                ],
            ]
            crossText = "".join(["".join(sorted(x)) for x in crossText])
            if crossText == "MSMS":
                xmasCount += 1
        except IndexError:
            continue
    print(xmasCount)


if __name__ == "__main__":
    p = Path(sys.argv[1])

    Part1(p)
    Part2(p)
