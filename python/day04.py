def read_input(filename):
    with open(filename, "r") as file:
        return [line.strip() for line in file.readlines()]
