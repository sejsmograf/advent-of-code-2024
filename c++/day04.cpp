#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> readInput() {
  std::vector<std::string> input;
  std::ifstream inputStream("../inputs/04.txt");
  std::string line;

  while (std::getline(inputStream, line)) {
    input.push_back(line);
  }

  return input;
}

int checkXmas(int row, int col, const std::vector<std::string> &source) {
  std::string search = "XMAS";
  std::pair<int, int> directions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  int sum = 0;
  for (int i = 0; i < 8; i++) {
    int dx = directions[i].first;
    int dy = directions[i].second;

    for (int letter_index = 1; letter_index < search.size(); letter_index++) {
      int newCol = col + letter_index * dx;
      int newRow = row + letter_index * dy;
      if (newCol < 0 || newCol >= source[0].size()) {
        break;
      }
      if (newRow < 0 || newRow >= source.size()) {
        break;
      }
      if (source[newRow][newCol] != search[letter_index]) {
        break;
      }
      if (letter_index == search.size() - 1) {
        sum++;
      }
    }
  }

  return sum;
}

bool checkMas(int row, int col, const std::vector<std::string> &source) {
  std::pair<int, int> directions[] = {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
  std::pair<int, int> opposite[] = {{-1, -1}, {1, 1}, {-1, 1}, {1, -1}};

  for (int i = 0; i < 4; i++) {
    int newRow = row + directions[i].first;
    int newCol = col + directions[i].second;
    int newRowOpposite = row + opposite[i].first;
    int newColOpposite = col + opposite[i].second;
    char one = source[newRow][newCol];
    char two = source[newRowOpposite][newColOpposite];
    if (!((one == 'M' && two == 'S') || (one == 'S' && two == 'M'))) {
      return false;
    }
  }
  return true;
}

int silver() {
  auto input = readInput();
  int sum = 0;
  for (int row = 0; row < input.size(); row++) {
    for (int col = 0; col < input[0].size(); col++) {
      if (input[row][col] == 'X') {
        sum += checkXmas(row, col, input);
      }
    }
  }
  return sum;
}

int gold() {
  auto input = readInput();
  int sum = 0;
  for (int row = 1; row < input.size() - 1; row++) {
    for (int col = 1; col < input[0].size() - 1; col++) {
      if (input[row][col] == 'A') {
        sum += checkMas(row, col, input);
      }
    }
  }
  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
