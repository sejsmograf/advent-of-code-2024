#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using Map = std::vector<std::string>;
using Pos = std::pair<int, int>;

Map readInput() {
  Map result;
  std::ifstream is("../inputs/20.txt");
  std::string line;

  while (std::getline(is, line)) {
    result.push_back(line);
  }

  is.close();
  return result;
}

std::pair<Pos, Pos> findStartEnd(const Map &map) {
  Pos start, end;
  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map[0].size(); col++) {
      if (map[row][col] == 'S') {
        start = {row, col};
      }
      if (map[row][col] == 'E') {
        end = {row, col};
      }
    }
  }
  return {start, end};
};

bool posInBounds(Pos pos, const Map &map) {
  return pos.first >= 0 && pos.first <= map.size() && pos.second >= 0 &&
         pos.second < map[0].size();
}

std::pair<std::vector<Pos>, std::map<Pos, int>> bfs(const Map &map) {
  auto [start, end] = findStartEnd(map);
  std::queue<std::pair<Pos, int>> toVisit;
  std::set<Pos> visited;
  std::map<Pos, int> distances;
  std::vector<Pos> visitingOrder;

  toVisit.push({start, 0});
  visited.insert(start);

  while (!toVisit.empty()) {
    Pos current = toVisit.front().first;
    int currentDistance = toVisit.front().second;
    distances.insert({current, currentDistance});
    visitingOrder.push_back(current);
    toVisit.pop();

    std::pair<int, int> directions[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (int i = 0; i < 4; i++) {
      std::pair<int, int> d = directions[i];
      Pos newPos = {current.first + d.first, current.second + d.second};
      if (!posInBounds(newPos, map) ||
          map[newPos.first][newPos.second] == '#') {
        continue;
      }

      if (!visited.contains(newPos)) {
        visited.insert(newPos);
        toVisit.push({newPos, currentDistance + 1});
      }
    }
  }

  return {visitingOrder, distances};
}

std::vector<int> findCheatsForPos(Pos pos, int picoseconds,
                                  const std::map<Pos, int> &distances) {
  int currentDistance = distances.at(pos);
  std::vector<int> cheats;

  for (const auto &entry : distances) {
    Pos newPos = entry.first;
    int newDist = entry.second;

    if (newDist <= currentDistance + picoseconds) {
      continue;
    }

    int manhattanDist =
        abs(pos.first - newPos.first) + abs(pos.second - newPos.second);

    if (manhattanDist > picoseconds) {
      continue;
    }

    cheats.push_back(newDist - manhattanDist - currentDistance);
  }

  return cheats;
}

int silver() {
  auto input = readInput();
  auto [order, distances] = bfs(input);

  std::vector<int> allCheats;
  for (const auto &pos : order) {
    auto cheats = findCheatsForPos(pos, 2, distances);
    allCheats.insert(allCheats.end(), cheats.begin(), cheats.end());
  }

  int count = 0;
  for (const auto &c : allCheats) {
    if (c >= 100) {
      count++;
    }
  }

  return count;
}

unsigned long long gold() {
  auto input = readInput();
  auto [order, distances] = bfs(input);

  std::vector<int> allCheats;
  for (const auto &pos : order) {
    auto cheats = findCheatsForPos(pos, 20, distances);
    allCheats.insert(allCheats.end(), cheats.begin(), cheats.end());
  }

  int count = 0;
  for (const auto &c : allCheats) {
    if (c >= 100) {
      count++;
    }
  }

  return count;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold();
}
