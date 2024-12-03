#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<int>> readInput() {
  std::vector<std::vector<int>> reports;
  std::vector<int> currentReport;
  std::ifstream input("../inputs/02.txt");
  std::string line;

  while (std::getline(input, line)) {
    currentReport.clear();
    std::stringstream sstream(line);
    int level;

    while (sstream >> level) {
      currentReport.push_back(level);
    }

    reports.push_back(currentReport);
  }

  return reports;
}

bool isReportSafe(const std::vector<int> &report) {
  bool increasing = report[0] < report[1];

  for (int i = 0; i < report.size() - 1; i++) {
    int diff = report[i] - report[i + 1];

    if (increasing) {
      if (diff < -3 || diff > -1) {
        return false;
      }
    } else {
      if (diff < 1 || diff > 3) {
        return false;
      }
    }
  }

  return true;
}

int silver() {
  auto input = readInput();
  int count = 0;
  for (int i = 0; i < input.size(); i++) {
    count += (int)isReportSafe(input[i]);
  }
  return count;
}

void printVector(const std::vector<int> &v) {
  for (int i = 0; i < v.size(); i++) {
    std::cout << v[i] << ", ";
  }
  std::cout << "\n";
}

int gold() {
  auto input = readInput();
  int count = 0;

  for (int i = 0; i < input.size(); i++) {
    if (isReportSafe(input[i])) {
      count++;
      continue;
    }

    for (int j = 0; j < input[i].size(); j++) {
      std::vector<int> copied(input[i]);
      copied.erase(copied.begin() + j);
      if (isReportSafe(copied)) {
        count++;
        break;
      }
    }
  }

  return count;
}


int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
