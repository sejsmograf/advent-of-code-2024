#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <string>
#include <vector>

using ul = unsigned long long;

std::vector<std::string> readInput() {
  std::ifstream is("../inputs/21.txt");
  std::vector<std::string> codes;
  std::string line;

  while (std::getline(is, line)) {
    codes.push_back(line);
  }

  is.close();
  return codes;
}

const std::map<char, std::vector<std::pair<char, char>>> numpadGraph = {
    {'7', {{'8', '>'}, {'4', 'v'}}},
    {'8', {{'7', '<'}, {'9', '>'}, {'5', 'v'}}},
    {'9', {{'8', '<'}, {'6', 'v'}}},
    {'4', {{'7', '^'}, {'5', '>'}, {'1', 'v'}}},
    {'5', {{'4', '<'}, {'8', '^'}, {'6', '>'}, {'2', 'v'}}},
    {'6', {{'9', '^'}, {'5', '<'}, {'3', 'v'}}},
    {'1', {{'4', '^'}, {'2', '>'}}},
    {'2', {{'1', '<'}, {'5', '^'}, {'3', '>'}, {'0', 'v'}}},
    {'3', {{'6', '^'}, {'2', '<'}, {'A', 'v'}}},
    {'0', {{'2', '^'}, {'A', '>'}}},
    {'A', {{'0', '<'}, {'3', '^'}}},
};

const std::map<std::pair<char, char>, std::string> shortest = {
    {{'A', 'A'}, ""},    {{'<', '<'}, ""},   {{'>', '>'}, ""},
    {{'^', '^'}, ""},    {{'v', 'v'}, ""},

    {{'A', '<'}, "v<<"}, {{'A', '>'}, "v"},  {{'A', 'v'}, "<v"},
    {{'A', '^'}, "<"},   {{'^', 'A'}, ">"},  {{'^', 'v'}, "v"},
    {{'^', '<'}, "v<"},  {{'^', '>'}, "v>"}, {{'<', 'v'}, ">"},
    {{'<', '>'}, ">>"},  {{'<', '^'}, ">^"}, {{'<', 'A'}, ">>^"},
    {{'v', '<'}, "<"},   {{'v', '>'}, ">"},  {{'v', '^'}, "^"},
    {{'v', 'A'}, "^>"},  {{'>', '<'}, "<<"}, {{'>', 'v'}, "<"},
    {{'>', '^'}, "<^"},  {{'>', 'A'}, "^"},
};

std::vector<std::string> findShortesPaths(
    char from, char to,
    const std::map<char, std::vector<std::pair<char, char>>> &graph) {
  std::queue<std::pair<char, std::string>> toVisit;
  std::map<char, int> minDistance;
  std::vector<std::string> optimalPaths;

  toVisit.push({from, ""});
  minDistance[from] = 0;

  while (!toVisit.empty()) {
    char current = toVisit.front().first;
    const std::string &currentPath = toVisit.front().second;
    int currentDistance = currentPath.size();
    toVisit.pop();

    if (minDistance.count(to) && currentDistance > minDistance[to]) {
      continue;
    }

    if (current == to) {
      if (optimalPaths.empty() || currentDistance == minDistance[to]) {
        optimalPaths.push_back(currentPath);
      }
      continue;
    }

    auto neighbors = graph.at(current);
    for (const auto &neighbor : neighbors) {
      char neighborNode = neighbor.first;
      char direction = neighbor.second;

      int newDistance = currentDistance + 1;

      if (!minDistance.count(neighborNode) ||
          newDistance <= minDistance[neighborNode]) {
        minDistance[neighborNode] = newDistance;
        toVisit.push({neighborNode, currentPath + direction});
      }
    }
  }

  return optimalPaths;
}

std::vector<std::string>
getSequences(const std::string &code,
             const std::map<char, std::vector<std::pair<char, char>>> &graph) {
  std::vector<std::string> previousOptimalPaths =
      findShortesPaths(code[0], code[1], graph);

  std::vector<std::string> concatenatedPaths;
  for (int i = 1; i < code.size() - 1; i++) {
    concatenatedPaths.clear();
    auto optimalPaths = findShortesPaths(code[i], code[i + 1], graph);

    for (const auto &path : previousOptimalPaths) {
      for (const auto &newPath : optimalPaths) {
        concatenatedPaths.push_back(path + "A" + newPath);
      }
    }
    previousOptimalPaths = concatenatedPaths;
  }

  std::for_each(concatenatedPaths.begin(), concatenatedPaths.end(),
                [](std::string &path) { path += "A"; });

  return concatenatedPaths;
}

void countAndPrintChars(const std::string &input) {
  std::unordered_map<char, int> charCount = {
      {'<', 0}, {'>', 0}, {'^', 0}, {'v', 0}, {'A', 0}};

  for (char c : input) {
    if (charCount.find(c) != charCount.end()) {
      charCount[c]++;
    }
  }

  std::cout << "Character counts:\n";
  for (const auto &pair : charCount) {
    std::cout << pair.first << ": " << pair.second << "\n";
  }
}

int extractNumericPart(const std::string &str) {
  int number = 0;

  if (sscanf(str.c_str(), "%dA", &number) == 1) {
    return number;
  }

  return 0;
}

std::string getShortestPath(const std::string &path) {
  std::string newPath;

  for (int i = 0; i < path.size() - 1; i++) {
    newPath.append(shortest.at({path[i], path[i + 1]}));
    newPath.push_back('A');
  }

  return newPath;
}

std::vector<std::string> partitionString(const std::string &str, int n) {
  std::vector<std::string> partitions;

  if (str.empty() || n <= 0)
    return partitions;
  if (n == 1) {
    partitions.push_back(str);
    return partitions;
  }

  int baseSize = str.size() / n;
  int remainder = str.size() % n;

  int startPos = 0;
  for (int i = 0; i < n; i++) {
    int partitionSize = baseSize + (i < remainder ? 1 : 0);

    if (startPos < str.size()) {
      int extraChar = (i < n - 1) ? 1 : 0;
      partitions.push_back(str.substr(startPos, partitionSize + extraChar));
      startPos += partitionSize;
    }
  }

  return partitions;
}

ul recursiveShortestPath(const std::string &path, int iterationsLeft,
                         std::map<std::pair<std::string, int>, ul> &memo) {
  if (memo.contains({path, iterationsLeft})) {
    return memo[{path, iterationsLeft}];
  }

  if (iterationsLeft == 0) {
    return path.size();
  }

  ul total = 0;

  auto parts = partitionString(path, 4);

  for (int i = 0; i < parts.size(); i++) {
    if (i == 0) {
      total += recursiveShortestPath(getShortestPath("A" + parts[i]),
                                     iterationsLeft - 1, memo);
    } else {
      total += recursiveShortestPath(getShortestPath(parts[i]),
                                     iterationsLeft - 1, memo);
    }
  }

  memo[{path, iterationsLeft}] = total;
  return total;
}

ul shortestPathLen(const std::string &code, int iterations) {
  auto paths = getSequences("A" + code, numpadGraph);
  ul min = std::numeric_limits<ul>().max();
  std::map<std::pair<std::string, int>, ul> memo;

  for (const auto &s : paths) {
    ul len = recursiveShortestPath(s, iterations, memo);
    min = std::min(min, len);
  }

  return min;
}

int silver() {
  auto input = readInput();
  int sum = 0;
  for (const auto &s : input) {
    sum += shortestPathLen(s, 2) * extractNumericPart(s);
  }

  return sum;
}

ul gold() {
  auto input = readInput();
  ul sum = 0;
  for (const auto &s : input) {
    sum += shortestPathLen(s, 25) * extractNumericPart(s);
  }

  return sum;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold();
}
