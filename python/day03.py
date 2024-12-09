import re


def read_input(filename: str) -> str:
    with open(filename, "r") as file:
        return "".join(file.readlines())


def silver() -> int:
    input = read_input("../inputs/03.txt")
    return sum([int(x) * int(y) for x, y in re.findall(r"mul\((\d*),(\d*)\)", input)])


def gold() -> int:
    input = read_input("../inputs/03.txt")
    matches = re.finditer(r"mul\((\d+),(\d+)\)|don't\(\)|do\(\)", input)
    sum = 0
    enabled = True
    for match in matches:
        if str(match.group()).startswith("don'"):
            enabled = False
        elif (str(match.group())).startswith("do"):
            enabled = True
        elif enabled:
            x, y = match.groups()
            sum += int(x) * int(y)

    return sum


if __name__ == "__main__":
    print(silver())
    print(gold())
