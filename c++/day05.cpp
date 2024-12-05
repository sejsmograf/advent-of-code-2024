#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Rules = std::vector<std::pair<int, int>>;
using Pages = std::vector<std::vector<int>>;

std::pair<Rules, Pages> readInput() {
  Rules rules;
  Pages pages;
  std::ifstream inputStream("../inputs/05.txt");
  std::string line;

  bool pagesStared = false;
  while (std::getline(inputStream, line)) {
    if (pagesStared) {

    } else {
      int left, right;
    }
  }

  return {rules, pages};
}

int silver() {
  readInput();
  return 0;
}

int main() { std::cout << "silver: " << silver() << "\n"; }
