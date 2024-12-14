import sys
from pathlib import Path


def Main(path: Path):
    p1Count = 0
    p2Count = 0
    for line in path.read_text().split("\n"):
        elList = [int(x) for x in line.split(" ")]
        safe = CheckIfSafe(elList)
        p1Count += 1 if safe else 0
        if not safe:
            for i in range(len(elList)):
                activeList = [int(x) for x in line.split(" ")]
                activeList.pop(i)
                safe = CheckIfSafe(activeList)
                if safe:
                    break
        p2Count += 1 if safe else 0
    print(p1Count, p2Count)


def CheckIfSafe(elList):
    direction = None
    safe = True
    for idx, val in enumerate(elList):
        if idx != 0:
            diff = val - elList[idx - 1]
            safe = safe and diff != 0
            safe = safe and abs(diff) <= 3
            if direction is not None and safe:
                safe = safe and ((diff // abs(diff)) == direction)
            direction = diff // abs(diff) if safe else None
        if not safe:
            break

    return safe


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
