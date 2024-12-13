import itertools
import sys
from pathlib import Path
from pprint import pp


def TurnRight(direction: tuple[int, int]) -> tuple[int, int]:
    x, y = direction
    newX = 1 if y != 0 else 0
    newX *= 1 if y >= 0 else -1
    newY = 1 if x != 0 else 0
    newY *= -1 if x >= 0 else 1
    return newX, newY


def GetRightVal(l, d, cMap):
    d = TurnRight(direction=d)
    rightIdx = (l[0] + d[0], l[1] + d[1])
    return cMap[rightIdx[0]][rightIdx[1]]


def RunToTarget(l, d, cMap, target):
    ReachedSpot = False
    while not ReachedSpot:
        nextSpot = (l[0] + d[0], l[1] + d[1])
        if nextSpot == target:
            ReachedSpot = True
        try:
            nextVal = cMap[nextSpot[0]][nextSpot[1]]
        except IndexError:
            break
        if nextVal == "#":
            turning = True
            d = TurnRight(direction=d)
        else:
            turning = False
            cMap[nextSpot[0]][nextSpot[1]] = "X"
            location = nextSpot
    return ReachedSpot


def Main(path: Path):
    charMap = [[char for char in line] for line in path.read_text().split("\n")]
    startIdx = path.read_text().replace("\n", "").find("^")
    startPos = startIdx // len(charMap[0]), startIdx % len(charMap[0])
    direction = (-1, 0)
    location = startPos
    spots = {location}
    loops = set()
    turning = False
    while True:
        nextSpot = (location[0] + direction[0], location[1] + direction[1])

        try:
            nextVal = charMap[nextSpot[0]][nextSpot[1]]
        except IndexError:
            break
        if nextVal == "#":
            turning = True
            direction = TurnRight(direction=direction)
        else:
            turning = False
            charMap[nextSpot[0]][nextSpot[1]] = "X"
            spots.add(nextSpot)
            location = nextSpot
    pp(charMap)
    print(FindHashes(path, charMap))
    print(len(spots), len(loops))


def FindHashes(path: Path, cMap):
    idxList = [idx for idx, x in enumerate(path.read_text().replace("\n", "")) if x == "#"]
    posList = []
    for i in idxList:
        posList.append((i // len(cMap[0]), i % len(cMap[0])))
    for p in itertools.permutations(posList, 3):
        print(p)


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
