#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

using Pos = std::pair<int, int>;
using Region = std::pair<char, std::pair<std::set<Pos>, int>>;
using Side = std::pair<int, bool>;

std::vector<std::string> readInput() {
  std::vector<std::string> result;
  std::ifstream inputStream("../inputs/12.txt");
  std::string line;

  while (std::getline(inputStream, line)) {
    result.push_back(line);
  }

  inputStream.close();
  return result;
}

bool isPosInBounds(Pos pos, const std::vector<std::string> &map) {
  int rows = map.size();
  int cols = map[0].size();
  return pos.first >= 0 && pos.first < rows && pos.second >= 0 &&
         pos.second < cols;
}

std::vector<Pos> getNeighbros(Pos pos, const std::vector<std::string> &map) {
  std::vector<Pos> result;
  std::pair<int, int> directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  for (int i = 0; i < 4; i++) {
    auto direction = directions[i];
    Pos newPos = {pos.first + direction.first, pos.second + direction.second};
    if (isPosInBounds(newPos, map)) {
      result.push_back(newPos);
    }
  }

  return result;
}

std::vector<Pos> getValidNeighbros(Pos pos, char ch,
                                   const std::vector<std::string> &map) {
  std::vector<Pos> result;
  std::vector<Pos> neighbors = getNeighbros(pos, map);

  for (const auto &n : neighbors) {
    if (map[n.first][n.second] == ch) {
      result.push_back(n);
    }
  }

  return result;
}

Region bfsRegion(Pos pos, std::vector<std::string> &map) {
  char targetChar = map[pos.first][pos.second];
  std::queue<Pos> toVisit;
  std::set<Pos> visited;
  toVisit.push(pos);
  int perimeter = 0;

  while (!toVisit.empty()) {
    Pos current = toVisit.front();
    toVisit.pop();
    if (visited.contains(current)) {
      continue;
    }

    visited.insert(current);

    auto neighbors = getValidNeighbros(current, targetChar, map);
    perimeter += (4 - neighbors.size());

    for (const auto &n : neighbors) {
      if (!visited.contains(n)) {
        toVisit.push(n);
      }
    }
  }

  for (const auto &pos : visited) {
    map[pos.first][pos.second] = '.';
  }

  return {targetChar, {visited, perimeter}};
}

int getRegionPrice(const Region &region) {
  // std::cout<< "region " << region.first << "\n";
  // std::cout<< "perimeter " << region.second.second << "\n";
  // std::cout<< "count " << region.second.first << "\n";
  // std::cout<< "\n";
  //
  return region.second.first.size() * region.second.second;
}

unsigned long long silver() {
  auto input = readInput();
  std::vector<Region> regions;

  for (int row = 0; row < input.size(); row++) {
    for (int col = 0; col < input[0].size(); col++) {
      if (input[row][col] != '.') {
        regions.push_back(bfsRegion({row, col}, input));
      }
    }
  }

  int sum = 0;
  for (const auto &region : regions) {
    sum += getRegionPrice(region);
  }

  return sum;
}

int getRegionSidesCount(const Region &region,
                        const std::vector<std::string> &map, bool horizontal) {

  int sidesCount = 0;
  std::set<int> prevInCols;
  std::set<int> prevOutCols;
  std::set<int> currInCols;
  std::set<int> currOutCols;
  bool inside;

  for (int row = 0; row <= map.size(); row++) {
    inside = false;
    prevInCols = currInCols;
    prevOutCols = currOutCols;
    currInCols.clear();
    currOutCols.clear();

    for (int col = 0; col <= map.size(); col++) {
      Pos pos = horizontal ? Pos{row, col} : Pos{col, row};
      if (!inside && region.second.first.contains(pos)) {
        inside = true;
        if (!prevInCols.contains(col)) {
          sidesCount++;
        }

        currInCols.insert(col);
      } else if (inside && !region.second.first.contains(pos)) {
        inside = false;
        if (!prevOutCols.contains(col)) {
          sidesCount++;
        }

        currOutCols.insert(col);
      }
    }
  }

  return sidesCount;
}

unsigned long long gold() {
  auto input = readInput();
  std::vector<Region> regions;

  for (int row = 0; row < input.size(); row++) {
    for (int col = 0; col < input[0].size(); col++) {
      if (input[row][col] != '.') {
        regions.push_back(bfsRegion({row, col}, input));
      }
    }
  }

  int sum = 0;
  for (const auto &region : regions) {
    int horizontal = getRegionSidesCount(region, input, true);
    int vertical = getRegionSidesCount(region, input, false);
    int sides = horizontal + vertical;
    int cost = sides * region.second.first.size();
    sum += cost;
  }

  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
