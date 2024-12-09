#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::string> readInput() {
  std::ifstream inputStream("../inputs/06.txt");
  std::vector<std::string> map;
  std::string line;

  while (std::getline(inputStream, line)) {
    map.emplace_back(line);
  }

  inputStream.close();
  return map;
}

std::pair<int, int> getGuardPos(const std::vector<std::string> &map) {
  char guardSymbol = '^';
  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map[0].size(); col++) {
      if (map[row][col] == guardSymbol) {
        return {row, col};
      }
    }
  }
  throw std::invalid_argument("Map must contain a guard");
}

std::pair<int, int> turnRight(std::pair<int, int> currentDirection) {
  return {currentDirection.second, -currentDirection.first};
}

std::set<std::pair<int, int>> traverseMap(const std::vector<std::string> &map) {
  std::set<std::pair<int, int>> visited;
  auto [row, col] = getGuardPos(map);
  std::pair<int, int> direction = {-1, 0};

  while (true) {
    visited.insert({row, col});

    int newRow = row + direction.first;
    int newCol = col + direction.second;
    if (newRow >= map.size() || newRow < 0 || newCol >= map[0].size() ||
        newCol < 0) {
      break;
    }

    if (map[newRow][newCol] == '#') {
      direction = turnRight(direction);
      continue;
    }
    row += direction.first;
    col += direction.second;
  }

  return visited;
}

bool detectLoop(const std::vector<std::string> &map) {
  std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> visited;
  auto [row, col] = getGuardPos(map);
  std::pair<int, int> direction = {-1, 0};

  while (true) {
    if (visited.contains({{row, col}, {direction}})) {
      return true;
    }

    visited.insert({{row, col}, {direction}});

    int newRow = row + direction.first;
    int newCol = col + direction.second;
    if (newRow >= map.size() || newRow < 0 || newCol >= map[0].size() ||
        newCol < 0) {
      return false;
    }

    if (map[newRow][newCol] == '#') {
      direction = turnRight(direction);
      continue;
    }
    row += direction.first;
    col += direction.second;
  }

  return false;
}

int traverseMapGold(const std::vector<std::string> &map) {
  std::set<std::pair<int, int>> visited = traverseMap(map);
  int toVisit = visited.size();
  std::pair<int, int> direction = {-1, 0};
  auto [row, col] = getGuardPos(map);

  int sum = 0;
  for (auto pos : visited) {
    std::vector<std::string> mapCopy(map);

    if (pos.first == row && pos.second == col) {
      continue;
    }

    mapCopy[pos.first][pos.second] = '#';

    if (detectLoop(mapCopy)) {
      sum ++;
      std::cout << "Loop detected at " << pos.first << ", " << pos.second << "\n";
    }
  }

  return sum;
}

int silver() {
  auto map = readInput();
  return traverseMap(map).size();
}

int gold() {
  auto map = readInput();
  return traverseMapGold(map);
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  // std::cout << "gold: " << gold() << "\n";
}
