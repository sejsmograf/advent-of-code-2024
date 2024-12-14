#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Button = std::pair<double, double>;
using Target = std::pair<double, double>;

struct Instruction {
  Button buttonA;
  Button buttonB;
  Target target;
};

std::vector<Instruction> readInput() {
  std::vector<Instruction> result;
  std::ifstream inputStream("../inputs/13.txt");
  std::string line;
  int partIndex = 0;

  Instruction current;
  while (std::getline(inputStream, line)) {
    unsigned long long x, y;
    switch (partIndex) {
    case 0:
      std::sscanf(line.c_str(), "Button A: X+%lld, Y+%lld", &x, &y);
      current.buttonA = std::make_pair(x, y);
      break;
    case 1:
      std::sscanf(line.c_str(), "Button B: X+%lld, Y+%lld", &x, &y);
      current.buttonB = std::make_pair(x, y);
      break;
    case 2:
      std::sscanf(line.c_str(), "Prize: X=%lld, Y=%lld", &x, &y);
      current.target = std::make_pair(x, y);
      result.push_back(current);
      current = {};
      break;
    }

    partIndex = (partIndex + 1) % 4;
  }

  inputStream.close();
  return result;
}

double findBCoefficient(const Instruction &instruction) {
  double coeff = (instruction.target.second * instruction.buttonA.first -
                  instruction.target.first * instruction.buttonA.second) /
                 (instruction.buttonB.second * instruction.buttonA.first -
                  instruction.buttonB.first * instruction.buttonA.second);

  return coeff;
}

std::pair<double, double> findIntersection(const Instruction &instruction) {
  double b = findBCoefficient(instruction);
  double a = (instruction.target.first - b * instruction.buttonB.first) /
             instruction.buttonA.first;
  return std::make_pair(a, b);
}

int silver() {
  auto input = readInput();
  int sum = 0;

  for (const auto &instruction : input) {
    auto intersection = findIntersection(instruction);
    if (intersection.first >= 0 && intersection.second >= 0 &&
        intersection.first <= 100 && intersection.second <= 100 &&
        std::abs(intersection.first - std::round(intersection.first)) <
            0.0001 &&
        std::abs(intersection.second - std::round(intersection.second)) <
            0.0001) {
      std::cout << intersection.first << " " << intersection.second << "\n";
      sum += intersection.first * 3 + intersection.second;
    }
  }

  return sum;
}

unsigned long long gold() {
  auto input = readInput();
  unsigned long long sum = 0;

  for (auto &instruction : input) {
    instruction.target.first += 10000000000000;
    instruction.target.second += 10000000000000;
    auto intersection = findIntersection(instruction);
    if (intersection.first >= 0 && intersection.second >= 0 &&
        intersection.first <= 100 && intersection.second <= 100 &&
        std::abs(intersection.first - std::round(intersection.first)) < 0.001 &&
        std::abs(intersection.second - std::round(intersection.second)) <
            0.001) {
      sum += intersection.first * 3 + intersection.second;
    }
  }

  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
