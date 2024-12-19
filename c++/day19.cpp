#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::pair<std::vector<std::string>, std::vector<std::string>> readInput() {
  std::vector<std::string> patterns;
  std::vector<std::string> towels;
  std::ifstream is("../inputs/19.txt");
  std::string line;

  bool towelsStarted;
  while (std::getline(is, line)) {
    if (line.empty()) {
      towelsStarted = true;
      continue;
    }
    if (towelsStarted) {
      towels.push_back(line);
    } else {
      std::stringstream ss(line);
      std::string pattern;
      while (std::getline(ss, pattern, ',')) {
        if (pattern.starts_with(" ")) {
          pattern.erase(0, 1);
        }
        patterns.push_back(pattern);
      }
    }
  }

  is.close();
  return {patterns, towels};
}

unsigned long long possiblePatternCombinations(
    const std::string &towel, const std::vector<std::string> &patterns,
    std::unordered_map<std::string, unsigned long long> &memo) {
  if (memo.contains(towel)) {
    return memo[towel];
  }
  unsigned long long total = 0;

  for (const std::string &pattern : patterns) {
    if (!towel.starts_with(pattern)) {
      continue;
    }

    if (pattern == towel) {
      total++;
    }

    std::string towelCopy(towel);
    towelCopy.erase(0, pattern.length());
    unsigned long long possible =
        possiblePatternCombinations(towelCopy, patterns, memo);
    total += possible;
  }

  memo.insert({towel, total});
  return total;
}

int silver() {
  auto input = readInput();
  auto patterns = input.first;

  int sum = 0;
  std::unordered_map<std::string, unsigned long long> memo;
  for (const std::string &towel : input.second) {
    if (possiblePatternCombinations(towel, patterns, memo)) {
      sum++;
    }
  }

  return sum;
}

unsigned long long gold() {
  auto input = readInput();

  auto patterns = input.first;

  unsigned long long sum = 0;
  std::unordered_map<std::string, unsigned long long> memo;
  for (const std::string &towel : input.second) {
    sum += possiblePatternCombinations(towel, patterns, memo);
  }
  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold();
}
