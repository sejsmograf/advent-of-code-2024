#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::string readInput() {
  std::string input;
  std::ifstream inputStream("../inputs/bigboy.txt");
  std::string line;

  while (std::getline(inputStream, line)) {
    input.append(line);
  }

  return input;
}

std::vector<std::string> getRegexMatches(const std::string &input,
                                         const std::regex &reg) {
  std::vector<std::string> res;

  std::smatch matches;
  std::regex_search(input, matches, reg);

  std::sregex_iterator regexIterator(input.begin(), input.end(), reg);
  std::sregex_iterator end;

  for (std::sregex_iterator k = regexIterator; k != end; ++k) {
    std::smatch match = *k;
    res.push_back(match.str());
  }

  return res;
}

int getMulResult(const std::string &mul) {
  int startIndex = mul.find('(');
  int endIndex = mul.find(",");
  int left = atoi(mul.substr(startIndex + 1, endIndex).c_str());
  startIndex = endIndex;
  endIndex = mul.find(')');
  int right = atoi(mul.substr(startIndex + 1, endIndex).c_str());

  return left * right;
}

int silver() {
  std::string input = readInput();
  std::regex reg("mul\\(\\d*,\\d*\\)");

  long sum = 0;
  std::vector<std::string> matches = getRegexMatches(input, reg);

  for (std::string match : matches) {
    sum += getMulResult(match);
  }

  return sum;
}

int gold() {
  std::string input = readInput();
  std::regex reg("mul\\(\\d*,\\d*\\)|do\\(\\)|don't\\(\\)");
  std::vector<std::string> matches = getRegexMatches(input, reg);

  bool enabled = true;
  long sum = 0;
  for (std::string match : matches) {
    if (match.substr(0, 3) == "do\(") {
      enabled = true;
      continue;
    } else if (match.substr(0, 3) == "don") {
      enabled = false;
      continue;
    }

    sum += enabled ? getMulResult(match) : 0;
  }

  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
