import sys
from pathlib import Path


def Main(path: Path): ...


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
