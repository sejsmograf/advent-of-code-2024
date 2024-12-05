def read_input(filename: str) -> tuple[list[int], list[int]]:
    result = ([], [])
    with open(filename, "r") as file:
        for line in file:
            words = line.strip().split()
            result[0].append(int(words[0]))
            result[1].append(int(words[1]))

    return result


def silver() -> int:
    input = read_input("../inputs/01.txt")
    fst = sorted(input[0])
    snd = sorted(input[1])

    return sum([abs(fst[i] - snd[i]) for i in range(len(fst))])


def gold() -> int:
    input = read_input("../inputs/01.txt")
    fst = input[0]
    snd = input[1]
    counts = {fst[i]: snd.count(fst[i]) for i in range(len(fst))}
    return sum([fst[i] * counts[fst[i]] for i in range(len(fst))])


if __name__ == "__main__":
    print(f"Silver: {silver()}")
    print(f"Gold: {gold()}")
