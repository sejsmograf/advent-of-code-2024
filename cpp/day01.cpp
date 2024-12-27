#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::pair<std::vector<int>, std::vector<int>> readInput() {
  std::vector<int> listOne;
  std::vector<int> listTwo;
  std::ifstream input("../inputs/01.txt");
  std::string line;

  while (std::getline(input, line)) {
    std::stringstream sstream(line);
    int left;
    int right;

    sstream >> left;
    sstream >> right;

    listOne.push_back(left);
    listTwo.push_back(right);
  }

  return {listOne, listTwo};
}

int calculateDistance(int left, int right) { return abs(right - left); }

int countOccurances(int value, const std::vector<int> &list) {
  return std::count(list.begin(), list.end(), value);
}

int silver() {
  auto input = readInput();

  std::sort(input.first.begin(), input.first.end());
  std::sort(input.second.begin(), input.second.end());

  int sum = 0;
  for (int i = 0; i < input.first.size(); i++) {
    sum += calculateDistance(input.first[i], input.second[i]);
  }

  return sum;
}

int gold() {
  auto input = readInput();

  long sum = 0;
  for (int i = 0; i < input.first.size(); i++) {
    int left = input.first[i];
    int right = input.second[i];

    sum += left * countOccurances(left, input.second);
  }
  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
