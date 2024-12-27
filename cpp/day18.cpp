#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

using Pos = std::pair<int, int>;
using Map = std::vector<std::string>;

std::vector<Pos> readInput() {
  std::vector<Pos> result;
  std::ifstream is("../inputs/18.txt");
  std::string line;

  while (std::getline(is, line)) {
    std::stringstream ss(line);
    int x, y;
    char sep;
    ss >> x >> sep >> y;
    result.push_back({y, x});
  }

  is.close();
  return result;
}

Map createMap(int dimension, const std::vector<Pos> &bytes, int bytesToFall) {
  Map result;
  for (int row = 0; row < dimension; row++) {
    std::string line;
    for (int col = 0; col < dimension; col++) {
      line.push_back('.');
    }
    result.push_back(line);
  }

  for (int i = 0; i < bytesToFall; i++) {
    Pos byte = bytes[i];
    result[byte.first][byte.second] = '#';
  }

  return result;
}

std::vector<Pos> getNeighbors(const Pos &current, const Map &map) {
  int rows = map.size();
  int cols = map.size();
  std::vector<Pos> neighbors;
  std::pair<int, int> directions[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  for (int i = 0; i < 4; i++) {
    std::pair<int, int> d = directions[i];
    Pos newPos = {current.first + d.first, current.second + d.second};

    if (newPos.first >= 0 && newPos.first < rows && newPos.second >= 0 &&
        newPos.second < cols && map[newPos.first][newPos.second] != '#') {
      neighbors.push_back(newPos);
    }
  }
  return neighbors;
}

int bfs(const Map &map) {
  Pos start = {0, 0};
  Pos end = {map.size() - 1, map.size() - 1};
  std::queue<std::pair<Pos, int>> toVisit;
  std::set<Pos> visited;

  toVisit.push({start, 0});

  while (!toVisit.empty()) {
    Pos current = toVisit.front().first;
    int currentDistance = toVisit.front().second;
    toVisit.pop();

    auto neighbors = getNeighbors(current, map);
    for (const Pos &n : neighbors) {
      if (n == end) {
        return currentDistance + 1;
      }
      if (!visited.contains(n)) {
        toVisit.push({n, currentDistance + 1});
        visited.insert(n);
      }
    }
  }

  return -1;
}

void printMap(const Map &map) {
  for (const std::string &line : map) {
    std::cout << line << "\n";
  }
}

int silver() {
  auto input = readInput();
  Map map = createMap(71, input, 1024);
  return bfs(map);
}

void gold() {
  auto input = readInput();

  for (int i = 0; i < input.size() + 1; i++) {
    Map map = createMap(71, input, i + 1);
    if (bfs(map) == -1) {
      std::cout << input[i].second << "," << input[i].first;
      return;
    }
  }
  throw std::invalid_argument("Failed to block path to exit");
}

int main() {
  // std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: ";
  gold();
}
