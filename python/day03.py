import re


def read_input(filename: str) -> str:
    with open(filename, "r") as file:
        return "".join(file.readlines())


def silver() -> int:
    input = read_input("../inputs/03.txt")
    return sum([int(x) * int(y) for x, y in re.findall(r"mul\((\d*),(\d*)\)", input)])


def gold() -> int:
    input = read_input("../inputs/03.txt")
    matches = re.findall(r"mul\((\d*),(\d*)\)|don't\(\)|do\(\)", input)
    sum = 0
    enabled = True
    for match in matches:
        if str(match).startswith("don'"):
            enabled = False
        elif (str(match)).startswith("do"):
            enabled = True
        elif enabled:
            x, y = match
            sum += int(x) * int(y)

    return 0


print(gold())
