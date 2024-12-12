from functools import lru_cache
import math


def read_input(filename):
    with open(filename, "r") as f:
        return [int(n) for n in f.read().split(" ")]


def get_digit_count(n):
    return 1 if n == 0 else math.floor(math.log10(n)) + 1


def split_number(n, digits):
    half = digits // 2
    split = 10**half
    return (n // split, n % split)


@lru_cache(maxsize=None)
def simulate_blink(stone, remaining):
    count = 0
    if remaining == 0:
        return 1

    if stone == 0:
        count += simulate_blink(1, remaining - 1)
    elif (digits := get_digit_count(stone)) % 2 == 0:
        splitted = split_number(stone, digits)
        count += simulate_blink(splitted[0], remaining - 1)
        count += simulate_blink(splitted[1], remaining - 1)
    else:
        count += simulate_blink(stone * 2024, remaining - 1)

    return count


def silver(input):
    return sum([simulate_blink(stone, 25) for stone in input])


def gold(input):
    return sum([simulate_blink(stone, 75) for stone in input])


if __name__ == "__main__":
    input = read_input("../inputs/11.txt")
    print(f"silver {silver(input)}")
    print(f"gold {gold(input)}")
