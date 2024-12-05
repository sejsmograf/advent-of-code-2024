def read_input(filename: str) -> list[list[int]]:
    with open(filename, "r") as file:
        return [[int(word) for word in line.strip().split()] for line in file]


def is_report_safe(report: list[int]) -> bool:
    incr: bool | None = None
    for i in range(len(report) - 1):
        diff = report[i] - report[i + 1]
        incr = incr if incr is not None else diff < 0
        if abs(diff) > 3 or abs(diff) == 0:
            return False
        if (incr and diff > 0) or (not incr and diff < 0):
            return False

    return True


def is_report_safe_gold(report: list[int]) -> bool:
    if is_report_safe(report):
        return True

    for i in range(len(report)):
        cp = list.copy(report)
        cp.pop(i)
        if is_report_safe(cp):
            return True

    return False


def silver() -> int:
    input = read_input("../inputs/02.txt")
    return sum([int(is_report_safe(report)) for report in input])


def gold() -> int:
    input = read_input("../inputs/02.txt")
    return sum([int(is_report_safe_gold(report)) for report in input])


if __name__ == "__main__":
    print(f"Silver: {silver()}")
    print(f"Gold: {gold()}")
