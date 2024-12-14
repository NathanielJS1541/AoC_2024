import re
import sys
from gc import enable
from pathlib import Path


def mul(a, b):
    return a * b


def Main(path: Path):
    text = path.read_text()
    p1Total = 0
    p2Total = 0
    enable = list(re.finditer(r"do(n't)?\(\)", text))
    for match in re.finditer(r"mul\(\d+,\d+\)", text):
        p1Total += eval(match.group())
        commandList = [x for x in enable if x.end() <= match.start()]
        if not commandList or "t" not in commandList[-1].group():
            p2Total += eval(match.group())
    print(p1Total, p2Total)


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
