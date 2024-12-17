#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

using Map = std::vector<std::string>;
using Pos = std::pair<int, int>;
using Direction = std::pair<int, int>;
using State = std::pair<Pos, Direction>;

Map readInput() {
  std::ifstream is("../inputs/16.txt");
  std::string line;
  Map map;

  while (std::getline(is, line)) {
    map.push_back(line);
  }

  is.close();
  return map;
}

void printMap(const Map &map) {
  std::cout << "\033[H";
  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map[0].size(); col++) {
      std::cout << map[row][col];
    }
    std::cout << "\n";
  }
}

std::pair<Pos, Pos> findStartEnd(const Map &map) {
  Pos start, end;
  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map[0].size(); col++) {
      if (map[row][col] == 'S') {
        start = {row, col};
      } else if (map[row][col] == 'E') {
        end = {row, col};
      }
    }
  }
  return {start, end};
}

struct compare {
  bool operator()(const std::pair<State, int> &a,
                  const std::pair<State, int> &b) {
    return a.second > b.second;
  }
};

std::set<State> getUniqueOnPath(
    const std::map<State, std::pair<std::vector<State>, int>> &previous,
    State current, bool isEnd, Pos start, std::set<State> currUniq) {

  Direction directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  if (current.first == start) {
    return currUniq;
  }

  currUniq.insert(current);
  std::vector<State> predecesors;

  if (isEnd) {
    for (Direction d : directions) {
      if (!previous.contains({current.first, d})) {
        continue;
      }

      for (State s : previous.at({current.first, d}).first) {
        predecesors.push_back(s);
      }
    }
  } else {
    if (!previous.contains(current)) {
      return currUniq;
    }

    for (State s : previous.at(current).first) {
      predecesors.push_back(s);
    }
  }

  for (auto s : predecesors) {
    if (!currUniq.contains(s)) {
      auto unique = getUniqueOnPath(previous, s, false, start, currUniq);
      for (auto u : unique) {
        currUniq.insert(u);
      }
    }
  }

  return currUniq;
}

int getRotationCost(Direction current, Direction target) {
  if (current == target) {
    return 0;
  } else if ((current.first == 0 && target.first == 0) ||
             (current.second == 0 && target.second == 0)) {
    return 2000;
  } else {
    return 1000;
  }
}

int dijkstra(const Map &map, bool gold) {
  auto [start, end] = findStartEnd(map);
  Direction currDir = {0, 1};
  std::set<State> visited;
  std::priority_queue<std::pair<State, int>, std::vector<std::pair<State, int>>,
                      compare>
      q;
  std::map<State, std::pair<std::vector<State>, int>> predecesors;

  q.push({{start, currDir}, 0});

  int bestCost = std::numeric_limits<int>().max();
  int numSolutions = 0;

  while (!q.empty()) {
    Pos current = q.top().first.first;
    Direction currDir = q.top().first.second;
    int currCost = q.top().second;
    visited.insert({current, currDir});
    q.pop();

    if (currCost > bestCost) {
      break;
    }

    Direction directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < 4; i++) {
      Direction targetDir = directions[i];
      int rotationCost = getRotationCost(currDir, targetDir);
      Pos neighbor = {current.first + targetDir.first,
                      current.second + targetDir.second};
      char at = map[neighbor.first][neighbor.second];
      if (at == '#') {
        continue;
      }

      if (rotationCost == 0) {
        int stepCost = currCost + 1;
        if (neighbor == end) {
          bestCost = stepCost;
          if(!gold) {
            return bestCost;
          }
        }

        if (!predecesors.contains({neighbor, currDir})) {
          predecesors.insert(
              {{neighbor, currDir}, {{{current, currDir}}, stepCost}});
        } else if (predecesors.at({neighbor, currDir}).second == stepCost) {
          predecesors.at({neighbor, currDir})
              .first.push_back({current, currDir});
        }

        if (!visited.contains({neighbor, currDir}) && neighbor != end) {
          q.push({{neighbor, currDir}, stepCost});
        }
      } else {
        int totalCost = currCost + rotationCost;
        if (!predecesors.contains({current, targetDir})) {
          predecesors.insert(
              {{current, targetDir}, {{{current, currDir}}, totalCost}});
        } else if (predecesors.at({current, targetDir}).second == totalCost) {
          predecesors.at({current, targetDir})
              .first.push_back({current, currDir});
        }
        if (!visited.contains({current, targetDir})) {
          q.push({{current, targetDir}, totalCost});
        }
      }
    }
  }

  auto sejsmo = getUniqueOnPath(predecesors, {end, {-1, -1}}, true, start, {});
  std::set<Pos> uniquePos;
  for (auto s : sejsmo) {
    uniquePos.insert(s.first);
  }

  return gold ? uniquePos.size() + 1 : bestCost;
}

unsigned long silver() {
  auto input = readInput();
  return dijkstra(input, false);
}

unsigned long long gold() {
  auto input = readInput();
  return dijkstra(input, true);
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
