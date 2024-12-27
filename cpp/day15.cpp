#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

using Map = std::vector<std::string>;
using Pos = std::pair<int, int>;
using Move = std::pair<int, int>;

std::pair<Map, std::vector<char>> readInput() {
  std::ifstream is("../inputs/15.txt");
  std::string line;
  std::vector<std::string> map;
  std::vector<char> moves;

  bool movesStarted = false;
  while (std::getline(is, line)) {
    if (!movesStarted) {
      if (line.empty()) {
        movesStarted = true;
        continue;
      }
      map.push_back(line);
    } else {
      for (int i = 0; i < line.size(); i++) {
        moves.push_back(line[i]);
      }
    }
  }

  is.close();
  return {map, moves};
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

Move moveFromChar(char ch) {
  switch (ch) {
  case '^':
    return {-1, 0};
  case 'v':
    return {1, 0};
  case '>':
    return {0, 1};
  case '<':
    return {0, -1};
  }
  throw std::invalid_argument("Invalid move character");
}

Pos findRobot(const Map &map) {
  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map[0].size(); col++) {
      if (map[row][col] == '@') {
        return {row, col};
      }
    }
  }
  throw std::invalid_argument("Map must contain a robot");
}

std::vector<Move> movesFromChars(const std::vector<char> &moves) {
  std::vector<Move> res;
  std::transform(moves.begin(), moves.end(), std::back_inserter(res),
                 [](char ch) { return moveFromChar(ch); });
  return res;
}

bool pushWall(Map &map, Pos currentPos, Move direction) {
  Pos nextPos = {currentPos.first + direction.first,
                 currentPos.second + direction.second};
  char atPos = map[currentPos.first][currentPos.second];

  if (atPos == 'O') {
    if (pushWall(map, nextPos, direction)) {
      map[currentPos.first][currentPos.second] = '.';
      map[nextPos.first][nextPos.second] = 'O';
      return true;
    }
    return false;
  } else if (atPos == '#') {
    return false;
  } else if (atPos == '.') {
    return true;
  }

  throw std::invalid_argument("Invalid map element");
}

bool pushWallHorizontal(Map &map, Pos currentPos, Move direction) {
  Pos nextPos = {currentPos.first + direction.first,
                 currentPos.second + direction.second};
  char atPos = map[currentPos.first][currentPos.second];

  if (atPos == '[' || atPos == ']') {
    if (pushWallHorizontal(map, nextPos, direction)) {
      map[currentPos.first][currentPos.second] = '.';
      map[nextPos.first][nextPos.second] = atPos;
      return true;
    }
    return false;
  } else if (atPos == '#') {
    return false;
  } else if (atPos == '.') {
    return true;
  }
  throw std::invalid_argument("Invalid character found");
}

Pos getComplementaryPos(const Map &map, Pos pos) {
  return map[pos.first][pos.second] == '[' ? Pos({pos.first, pos.second + 1})
                                           : Pos({pos.first, pos.second - 1});
}

bool pushWallVertical(Map &map, const std::vector<Pos> &toMove,
                      Move direction) {
  bool possibleToMove = true;
  for (const auto p : toMove) {
    Pos newPos = {p.first + direction.first, p.second + direction.second};
    char atNewPos = map[newPos.first][newPos.second];
    if (atNewPos == '#') {
      return false;
    }
    if (atNewPos != '.') {
      possibleToMove = false;
      break;
    }
  }
  if (possibleToMove) {
    return true;
  }

  std::set<Pos> newToMove;
  for (const auto p : toMove) {
    Pos newPos = {p.first + direction.first, p.second + direction.second};
    if (map[newPos.first][newPos.second] == '[' ||
        map[newPos.first][newPos.second] == ']') {
      newToMove.insert(newPos);
      newToMove.insert(getComplementaryPos(map, newPos));
    }
  }

  if (pushWallVertical(map,
                       std::vector<Pos>(newToMove.begin(), newToMove.end()),
                       direction)) {
    for (const auto p : newToMove) {
      char atoldPos = map[p.first][p.second];
      Pos newPos = {p.first + direction.first, p.second + direction.second};
      map[p.first][p.second] = '.';
      map[newPos.first][newPos.second] = atoldPos;
    }
    return true;
  }
  return false;
}

bool pushWallGold(Map &map, Pos currentPos, Move direction) {
  if (direction.first == 0) {
    return pushWallHorizontal(map, currentPos, direction);
  } else {
    std::vector<Pos> toMove = {currentPos,
                               getComplementaryPos(map, currentPos)};
    if (pushWallVertical(map, toMove, direction)) {
      for (const auto p : toMove) {
        char atoldPos = map[p.first][p.second];
        Pos newPos = {p.first + direction.first, p.second + direction.second};
        map[p.first][p.second] = '.';
        map[newPos.first][newPos.second] = atoldPos;
      }
      return true;
    }
    return false;
  }
}

void moveRobot(Map &map, Move move) {
  Pos robotPos = findRobot(map);
  Pos newPos = {robotPos.first + move.first, robotPos.second + move.second};
  char atNewPos = map[newPos.first][newPos.second];

  switch (atNewPos) {
  case ('#'):
    return;
  case ('.'):
    map[robotPos.first][robotPos.second] = '.';
    map[newPos.first][newPos.second] = '@';
    return;
  case ('O'):
    if (pushWall(map, newPos, move)) {
      map[robotPos.first][robotPos.second] = '.';
      map[newPos.first][newPos.second] = '@';
    }
    return;
  case ('['):
  case (']'):
    if (pushWallGold(map, newPos, move)) {
      map[robotPos.first][robotPos.second] = '.';
      map[newPos.first][newPos.second] = '@';
    };
    return;
  }
}

unsigned long long sumBoxes(const Map &map) {
  unsigned long long sum = 0;
  for (int row = 0; row < map.size(); row++) {
    for (int col = 0; col < map[0].size(); col++) {
      if (map[row][col] == 'O' || map[row][col] == '[') {
        sum += row * 100 + col;
      }
    }
  }
  return sum;
}

Map resizeMap(const Map &map) {
  Map resized;
  for (int row = 0; row < map.size(); row++) {
    std::string mapRow;
    for (int col = 0; col < map[0].size(); col++) {
      char at = map[row][col];
      if (at == '#') {
        mapRow.append("##");
      } else if (at == '.') {
        mapRow.append("..");
      } else if (at == 'O') {
        mapRow.append("[]");
      } else if (at == '@') {
        mapRow.append("@.");
      }
    }
    resized.push_back(mapRow);
  }

  return resized;
}

unsigned long silver() {
  auto input = readInput();
  auto map = input.first;
  auto moves = movesFromChars(input.second);

  for (auto m : moves) {
    moveRobot(map, m);
  }

  return sumBoxes(map);
}

unsigned long long gold() {
  auto input = readInput();
  auto map = resizeMap(input.first);
  auto moves = movesFromChars(input.second);

  for (auto m : moves) {
    moveRobot(map, m);
  }

  return sumBoxes(map);
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
