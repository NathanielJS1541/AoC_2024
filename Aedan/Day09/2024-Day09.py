import sys
from pathlib import Path


def CreateHDD(string: str) -> list[str | int]:  #
    outVal: list[str | int] = []
    index = 0
    for idx, s in enumerate(string):
        if idx % 2 == 0:
            outVal += [index] * int(s)
            index += 1
        else:
            outVal += ["."] * int(s)

    return outVal


def Main(path: Path):
    longForm = CreateHDD(path.read_text())

    shortForm = ShrinkHDD(longForm)
    shortBlockForm = ShrinkHDDBlocks(longForm)
    resultP1 = CheckSum(shortForm)
    resultP2 = CheckSum(shortBlockForm)
    print(resultP1, resultP2)


def CheckSum(hddList):
    sumTotal = 0
    for idx, el in enumerate(hddList):
        if isinstance(el, int):
            sumTotal += idx * el

    return sumTotal


def ReformatHDDList(hddList):
    currentBlock = None
    count = 0
    result = []
    for x in hddList:
        if x != currentBlock:
            if currentBlock is not None:
                result.append((currentBlock, count))
            currentBlock = x
            count = 0
        count += 1
    result.append((currentBlock, count))
    return result


def ShrinkHDDBlocks(hddList):
    hddList = ReformatHDDList(hddList)
    for idx in reversed(range(len(hddList))):
        if hddList[idx][0] != ".":
            size = hddList[idx][1]
            for dstIdx, i in enumerate(hddList):
                if dstIdx > idx:
                    break
                if i[0] == "." and i[1] >= size:
                    newVal = hddList[idx]
                    if (hddList[dstIdx][1] - size) > 0:
                        hddList[dstIdx] = (hddList[dstIdx][0], hddList[dstIdx][1] - size)
                        hddList[idx] = (".", newVal[1])
                        hddList.insert(dstIdx, newVal)
                    else:
                        hddList[idx] = (".", newVal[1])
                        hddList.insert(dstIdx, newVal)
                        hddList.pop(dstIdx + 1)
                    # print("".join([str(x[0]) * x[1] for x in hddList if x[1] > 0]))
                    # x = input()
                    break

    result = "".join([str(x[0]) * x[1] for x in hddList if x[1] > 0])
    return [int(x) if x.isnumeric() else x for x in result]


def ShrinkHDD(hddList):
    currentIdx = len(hddList) - 1
    result = ["."] * len(hddList)
    for idx in range(len(hddList)):
        if currentIdx < idx:
            break
        if hddList[idx] == ".":
            while hddList[currentIdx] == ".":
                currentIdx -= 1
            result[idx] = hddList[currentIdx]
            currentIdx -= 1
        else:
            result[idx] = hddList[idx]
    result = [x for x in result if x != "."]
    return result


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
