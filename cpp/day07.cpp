#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Line = std::pair<unsigned long long, std::vector<unsigned long long>>;

std::vector<Line> readInput() {
  std::vector<Line> result;
  std::ifstream inputStream("../inputs/07.txt");
  unsigned long long target;
  std::vector<unsigned long long> numbers;
  std::string line;

  while (std::getline(inputStream, line)) {
    std::vector<unsigned long long> numbers;
    std::stringstream ss(line);

    char sep;
    unsigned long long num;
    ss >> target;
    ss >> sep;

    while (ss >> num) {
      numbers.push_back(num);
    }

    result.push_back({target, numbers});
  }

  inputStream.close();
  return result;
}

bool isLineSolvableSilver(const Line &line, unsigned long long current, unsigned long long currentIndex) {
  if (currentIndex == 0) {
    return isLineSolvableSilver(line, line.second[0], currentIndex + 1);
  }

  if (currentIndex == line.second.size()) {
    return current == line.first;
  }

  unsigned long long atIndex = line.second[currentIndex];
  return isLineSolvableSilver(line, current * atIndex, currentIndex + 1) ||
         isLineSolvableSilver(line, current + atIndex, currentIndex + 1);
}

unsigned long long concatenateInts(unsigned long long a, unsigned long long b) {
  unsigned long long temp = b;
  while (temp > 0) {
    a *= 10;
    temp /= 10;
  }
  return a + b;
}

bool isLineSolvableGold(const Line &line, unsigned long long current, unsigned long long currentIndex) {
  if (currentIndex == 0) {
    return isLineSolvableGold(line, line.second[0], currentIndex + 1);
  }

  if (currentIndex == line.second.size()) {
    return current == line.first;
  }

  unsigned long long atIndex = line.second[currentIndex];
  return isLineSolvableGold(line, current * atIndex, currentIndex + 1) ||
         isLineSolvableGold(line, current + atIndex, currentIndex + 1) ||
         isLineSolvableGold(line, concatenateInts(current, atIndex), currentIndex + 1);
}

unsigned long long silver() {
  auto input = readInput();

  unsigned long long sum = 0;
  for (auto line : input) {
    if (isLineSolvableSilver(line, 0, 0)) {
      sum += line.first;
    }
  }

  return sum;
}

unsigned long long gold() {
  auto input = readInput();

  unsigned long long sum = 0;
  for (auto line : input) {
    if (isLineSolvableGold(line, 0, 0)) {
      sum += line.first;
    }
  }

  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
