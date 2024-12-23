#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using ul = unsigned long long;

std::vector<ul> readInput() {
  std::vector<ul> result;
  std::ifstream is("../inputs/22.txt");
  std::string line;

  ul n;
  while (std::getline(is, line)) {
    std::stringstream ss(line);
    ss >> n;
    result.push_back(n);
  }

  is.close();
  return result;
}

ul mix(ul a, ul b) { return a ^ b; }

ul prune(ul a) { return a % 16777216; }

ul silver() {
  auto input = readInput();
  std::vector<ul> result;

  for (int i = 0; i < input.size(); i++) {
    ul price = input[i];
    for (int j = 0; j < 2000; j++) {
      price = prune(mix(price, price << 6));
      price = prune(mix(price, price >> 5));
      price = prune(mix(price, price << 11));
    }
    result.push_back(price);
  }

  ul sum = 0;
  for (const auto price : result) {
    sum += price;
  }

  return sum;
}

ul gold() {
  auto input = readInput();
  std::vector<ul> result;
  std::map<std::vector<int>, int> uniqueDiffs;

  for (int i = 0; i < input.size(); i++) {
    ul number = input[i];
    int price = 0;
    int prevPrice = (int)number % 10;
    std::vector<int> diffs;
    std::set<std::vector<int>> uniqueThisNumber;
    for (int j = 0; j < 2000; j++) {
      if (diffs.size() == 4) {
        std::vector<int> key(diffs.begin(), diffs.begin() + 4);
        if (!uniqueThisNumber.contains(key)) {
          uniqueThisNumber.insert(key);
          if (!uniqueDiffs.contains(key)) {
            uniqueDiffs.insert({key, (int)number % 10});
          } else {
            uniqueDiffs[key] += (int) number % 10;
          }
        }
        diffs.erase(diffs.begin());
      }

      number = prune(mix(number, number << 6));
      number = prune(mix(number, number >> 5));
      number = prune(mix(number, number << 11));

      price = (int)number % 10;
      int diff = price - prevPrice;
      prevPrice = price;
      diffs.push_back(diff);
    }
  }

  int max = 0;
  for (const auto &entry : uniqueDiffs) {
    max = std::max(max, entry.second);
  }

  return max;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold();
}
