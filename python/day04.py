def read_input(filename):
    with open(filename, "r") as file:
        return [line.strip() for line in file.readlines()]


def check_xmas(row, col, source):
    search = "XMAS"
    directions = [[0, 1], [0, -1], [1, 0], [-1, 0], [1, 1], [-1, -1], [1, -1], [-1, 1]]

    sum = 0
    for d in directions:
        for letter_index in range(1, len(search)):
            new_row = row + d[0]
            new_col = col + d[1]
            if (
                new_col < 0
                or new_col >= len(source[0])
                or new_row < 0
                or new_row < len(source)
            ):
                break
            if source[new_row][new_col] != search[letter_index]:
                sum += 1

    return sum


def silver():
    input = read_input("../inputs/04.txt")

    return sum
