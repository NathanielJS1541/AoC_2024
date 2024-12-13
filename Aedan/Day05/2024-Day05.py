import sys
from pathlib import Path


def Pt1(rules, tests):
    passedTests = []
    for test in tests:
        passed = True
        for rule in rules:
            page, before = rule
            if page in test and before in test:
                if test.index(page) > test.index(before):
                    passed = False
                    break
        if passed:
            passedTests.append(test)
    middleVals = [x[(len(x) - 1) // 2] for x in passedTests]
    return sum(middleVals), passedTests


def Pt2(rules, failedTests: list[list]):
    passedTests = []
    for test in failedTests:
        test = CheckRules(rules, test)
        passedTests.append(test)
    middleVals = [x[(len(x) - 1) // 2] for x in passedTests]
    return sum(middleVals)


def CheckRules(rules, test):
    for ruleNum, rule in enumerate(rules):
        page, before = rule
        if page in test and before in test:
            if test.index(page) > test.index(before):
                numPopped = test.count(page)
                test = [x for x in test if x != page]
                for _ in range(numPopped):
                    test.insert(test.index(before), page)
                test = CheckRules(rules[:ruleNum], test)
    return test


def Main(path: Path):
    rules = []
    tests: list[list[int]] = []
    for line in path.read_text().split("\n"):
        if line != "":
            lineList = [int(x) for x in line.split("|" if "|" in line else ",")]
            rules.append(lineList) if "|" in line else tests.append(lineList)
    p1Result, passedTests = Pt1(rules, tests)
    p2Result = Pt2(rules, [x for x in tests if x not in passedTests])

    print(p1Result)
    print(p2Result)


if __name__ == "__main__":
    p = Path(sys.argv[1])
    Main(p)
