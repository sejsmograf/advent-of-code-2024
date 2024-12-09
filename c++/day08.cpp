#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using Pos = std::pair<int, int>;

std::vector<std::string> readInput() {
  std::ifstream inputStream("../inputs/08.txt");
  std::vector<std::string> result;
  std::string line;

  while (std::getline(inputStream, line)) {
    result.push_back(line);
  }

  inputStream.close();
  return result;
}

std::map<char, std::vector<Pos>>
getAntenas(const std::vector<std::string> &input) {
  std::map<char, std::vector<Pos>> result;

  for (int row = 0; row < input.size(); row++) {
    for (int col = 0; col < input.size(); col++) {
      char ch = input[row][col];
      if (ch != '.') {
        if (!result.contains(ch)) {
          result.insert({ch, {}});
        }
        result.at(ch).push_back({row, col});
      }
    }
  }

  return result;
}

std::pair<Pos, Pos> getAntinodeLocation(const Pos &one, const Pos &two) {
  int rowDiff = one.first - two.first;
  int colDiff = one.second - two.second;

  return {{one.first + rowDiff, one.second + colDiff},
          {two.first - rowDiff, two.second - colDiff}};
}

std::vector<Pos> getNodesForAntena(const std::vector<Pos> &antenas) {
  std::vector<Pos> result;
  for (int i = 0; i < antenas.size() - 1; i++) {
    for (int j = i + 1; j < antenas.size(); j++) {
      auto antinodes = getAntinodeLocation(antenas[i], antenas[j]);
      result.push_back(antinodes.first);
      result.push_back(antinodes.second);
    }
  }

  return result;
}

bool isPosInBounds(const Pos &pos, int rows, int cols) {
  return pos.first >= 0 && pos.first < rows && pos.second >= 0 &&
         pos.second < cols;
}

std::vector<Pos> getAntinodeLocationGold(const Pos &one, const Pos &two, int rows, int cols) {
  std::vector<Pos> result;
  result.push_back(one);
  result.push_back(two);

  int rowDiff = one.first - two.first;
  int colDiff = one.second - two.second;
  Pos nodeCandidate = {one.first + rowDiff, one.second + colDiff};

  while(isPosInBounds(nodeCandidate, rows, cols)) {
    result.push_back(nodeCandidate);
    nodeCandidate = {nodeCandidate.first + rowDiff, nodeCandidate.second + colDiff};
  }

  nodeCandidate = {two.first - rowDiff, two.second - colDiff};

  while(isPosInBounds(nodeCandidate, rows, cols)) {
    result.push_back(nodeCandidate);
    nodeCandidate = {nodeCandidate.first - rowDiff, nodeCandidate.second - colDiff};
  }

  return result;
}

std::vector<Pos> getNodesForAntenaGold(const std::vector<Pos> &antenas, int rows, int cols) {
  std::vector<Pos> result;
  for (int i = 0; i < antenas.size() - 1; i++) {
    for (int j = i + 1; j < antenas.size(); j++) {
      auto antinodes = getAntinodeLocationGold(antenas[i], antenas[j], rows, cols);
      for (Pos node : antinodes) {
        result.push_back(node);
      }
    }
  }

  return result;
}


unsigned long long silver() {
  auto input = readInput();
  auto antenas = getAntenas(input);
  std::set<Pos> allAntinodes;
  int rows = input.size();
  int cols = input[0].size();

  for (auto entry : antenas) {
    std::vector<Pos> antinodes = getNodesForAntena(entry.second);
    for (const Pos& node : antinodes) {
      if (isPosInBounds(node,  rows, cols)) {
        allAntinodes.insert(node);
      }
    }
  }

  return allAntinodes.size();
}

unsigned long long gold() {
  auto input = readInput();
  auto antenas = getAntenas(input);
  std::set<Pos> allAntinodes;
  int rows = input.size();
  int cols = input[0].size();

  for (auto entry : antenas) {
    std::vector<Pos> antinodes = getNodesForAntenaGold(entry.second, rows, cols);
    for (const Pos& node : antinodes) {
      if (isPosInBounds(node,  rows, cols)) {
        allAntinodes.insert(node);
      }
    }
  }

  return allAntinodes.size();

}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
