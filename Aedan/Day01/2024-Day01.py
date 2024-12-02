import sys
from pathlib import Path


def Main(path: Path):
    l1, l2 = ReadList(path)
    sumDist = calcDistance(l1, l2)
    similarityScore = calcSim(l1, l2)
    print(sumDist, similarityScore)


def ReadList(path) -> tuple[list[int], list[int]]:
    l1: list[int] = []
    l2: list[int] = []
    for line in path.read_text().split("\n"):
        val1, val2 = [x.strip() for x in line.split("   ")]
        l1.append(int(val1))
        l2.append(int(val2))
    return l1, l2


def calcSim(l1, l2):
    count = 0
    for el in l1:
        count += l2.count(el) * el
    return count


def calcDistance(l1, l2) -> int:
    l1 = sorted(l1)
    l2 = sorted(l2)
    sumDist = 0
    for v1, v2 in zip(l1, l2):
        sumDist += abs(v1 - v2)
    return sumDist


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
