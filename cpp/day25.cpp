#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using Lock = std::vector<std::string>;

std::pair<std::vector<Lock>, std::vector<Lock>> readInput() {
  std::vector<Lock> locks;
  std::vector<Lock> keys;
  std::ifstream is("../inputs/25.txt");
  std::string line;

  bool gatesStarted = false;
  while (std::getline(is, line)) {
    std::vector<std::string> instance;
    for (int i = 0; i < 7; i++) {
      instance.push_back(line);
      std::getline(is, line);
    }
    if (instance[0].starts_with("#")) {
      locks.push_back(instance);
    } else {
      keys.push_back(instance);
    }
  }

  is.close();
  return {locks, keys};
}

std::vector<int> getLockPinHeights(const Lock &lock) {
  std::vector<int> result(5, 0);
  for (int row = 1; row < lock.size() - 1; row++) {
    for (int col = 0; col < lock.size(); col++) {
      char at = lock[row][col];
      if (at == '#') {
        result[col]++;
      }
    }
  }

  return result;
}

std::vector<int> getKeyPinHeights(const Lock &lock) {
  std::vector<int> result(5, 0);

  for (int row = lock.size() - 2; row > 0; row--) {
    for (int col = 0; col < lock.size(); col++) {
      char at = lock[row][col];
      if (at == '#') {
        result[col]++;
      }
    }
  }

  return result;
}

void printLock(const Lock &lock) {
  for (const auto &line : lock) {
    std::cout << line << "\n";
  }
  std::cout << "\n";
}

int silver() {
  auto input = readInput();
  auto locks = input.first;
  auto keys = input.second;

  std::vector<std::vector<int>> lockPins;
  for (auto lock : locks) {
    auto lockPinHeights = getLockPinHeights(lock);
    lockPins.push_back(lockPinHeights);
  }
  int sum = 0;

  for (const auto &lock : lockPins) {
    for (const auto &key : keys) {
      bool fits = true;
      auto keyPinHeights = getKeyPinHeights(key);
      for (int i = 0; i < lock.size(); i++) {
        if (lock[i] + keyPinHeights[i] > 5) {
          fits = false;
          break;
        }
      }
      if (fits) {
        sum++;
      }
    }
  }

  return sum;
}

int gold() { return 0; }

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold();
}
