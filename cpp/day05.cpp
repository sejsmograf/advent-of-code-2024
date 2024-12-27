#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using RulesInput = std::vector<std::pair<int, int>>;
using Pages = std::vector<std::vector<int>>;
using Rules = std::unordered_map<int, std::set<int>>;

std::pair<int, int> parseRule(const std::string &line) {
  std::stringstream ss(line);
  int left, right;
  char sep;
  ss >> left >> sep >> right;
  return {left, right};
}

std::vector<int> parsePage(const std::string &line) {
  std::vector<int> page;
  std::stringstream ss(line);
  int n;
  ss >> n;
  page.push_back(n);
  char sep;
  while (ss >> sep) {
    ss >> n;
    page.push_back(n);
  }
  return page;
}

std::pair<RulesInput, Pages> readInput() {
  RulesInput rules;
  Pages pages;
  std::ifstream inputStream("../inputs/05.txt");
  std::string line;

  bool pagesStared = false;
  while (std::getline(inputStream, line)) {
    if (line.empty()) {
      pagesStared = true;
      continue;
    }
    if (pagesStared) {
      pages.emplace_back(parsePage(line));
    } else {
      rules.emplace_back(parseRule(line));
    }
  }

  return {rules, pages};
}

Rules getRules(const RulesInput &rulesInput, bool before) {
  Rules rules;
  for (int i = 0; i < rulesInput.size(); i++) {
    int associated = before ? rulesInput[i].first : rulesInput[i].second;
    int rule = before ? rulesInput[i].second : rulesInput[i].first;

    if (!rules.contains(associated)) {
      rules.insert({associated, {}});
    }
    rules.at(associated).insert(rule);
  }
  return rules;
}

bool isUpdateValid(const std::vector<int> &update, const Rules &rulesBefore,
                   const Rules &rulesAfter) {
  for (int i = 0; i < update.size(); i++) {
    int invalidator = update[i];
    if (i < update.size() - 1) {
      for (int j = i + 1; j < update.size(); j++) {
        int toCheck = update[j];
        if (rulesBefore.contains(toCheck) &&
            rulesBefore.at(toCheck).contains(invalidator)) {
          return false;
        }
      }
    }
    if (i > 0) {
      for (int j = i - 1; j >= 0; j--) {
        int toCheck = update[j];
        if (rulesAfter.contains(toCheck) &&
            rulesAfter.at(toCheck).contains(invalidator)) {
          return false;
        }
      }
    }
  }
  return true;
}

void sortUpdate(std::vector<int> &update, const Rules &before,
                const Rules &after) {

  std::sort(update.begin(), update.end(), [&before](int a, int b) {
    return before.contains(a) && before.at(a).contains(b);
  });
}

int silver() {
  auto input = readInput();
  Rules before = getRules(input.first, true);
  Rules after = getRules(input.first, false);

  int sum = 0;
  for (int i = 0; i < input.second.size(); i++) {
    std::vector<int> update = input.second[i];
    if (isUpdateValid(update, before, after)) {
      sum += update[update.size() / 2];
    }
  }

  return sum;
}

int gold() {
  auto input = readInput();
  Rules before = getRules(input.first, true);
  Rules after = getRules(input.first, false);

  int sum = 0;
  for (int i = 0; i < input.second.size(); i++) {
    std::vector<int> update = input.second[i];
    if(isUpdateValid(update, before, after)) {
      continue;
    }

    sortUpdate(update, before, after);
    sum += update[update.size() / 2];
  }

  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
