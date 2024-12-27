#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using Map = std::vector<std::vector<int>>;
using Pos = std::pair<int, int>;

Map readInput() {
  Map result;
  std::ifstream inputStream("../inputs/10.txt");
  std::string line;
  int rowIndex = 0;

  while (std::getline(inputStream, line)) {
    result.push_back({});
    for (int i = 0; i < line.size(); i++) {
      result[rowIndex].push_back(line[i] - '0');
    }
    rowIndex++;
  }

  inputStream.close();
  return result;
}

bool isPosValid(Pos pos, const Map &map) {
  return pos.first >= 0 && pos.first < map.size() && pos.second >= 0 &&
         pos.second < map[0].size();
}

std::vector<Pos> getValidNeighbors(Pos pos, const Map &map) {
  std::vector<std::pair<int, int>> directions = {
      {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  std::vector<Pos> valid;

  for (auto d : directions) {
    Pos neighbor = {pos.first + d.first, pos.second + d.second};
    if (isPosValid(neighbor, map)) {
      valid.push_back(neighbor);
    }
  }
  return valid;
}

int startBfsHike(Pos pos, const Map &map, bool gold) {
  int trailsFound = 0;
  std::set<Pos> visited;
  std::queue<Pos> toVisit;
  toVisit.push(pos);
  int sum = 0;

  while (!toVisit.empty()) {
    Pos current = toVisit.front();
    toVisit.pop();
    int cValue = map[current.first][current.second];
    auto neighbors = getValidNeighbors(current, map);

    for (auto n : neighbors) {
      if (!gold && visited.contains(n)) {
        continue;
      }
      int nValue = map[n.first][n.second];
      if (nValue == cValue + 1) {
        toVisit.push(n);
        visited.insert(n);
        if (nValue == 9) {
          sum++;
          continue;
        }
      }
    }
  }
  return sum;
}


unsigned long long silver() { 
  Map map = readInput();
  int sum = 0;

  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map.size(); col++) {
      if(map[row][col] == 0) {
        int score = startBfsHike({row, col}, map, false);
        sum += score;
      }
    }
  }

  return sum;
}

unsigned long long gold() {
  Map map = readInput();
  int sum = 0;

  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map.size(); col++) {
      if(map[row][col] == 0) {
        int score = startBfsHike({row, col}, map, true);
        sum += score;
      }
    }
  }

  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
