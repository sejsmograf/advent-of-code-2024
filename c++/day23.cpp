#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using Graph = std::map<std::string, std::vector<std::string>>;

std::vector<std::pair<std::string, std::string>> readInput() {
  std::vector<std::pair<std::string, std::string>> result;
  std::ifstream is("../inputs/23.txt");
  std::string line;

  std::string from, to;
  while (std::getline(is, line)) {
    std::stringstream ss(line);
    getline(ss, from, '-');
    getline(ss, to);
    result.push_back({from, to});
  }

  is.close();
  return result;
}

Graph createGraph(
    const std::vector<std::pair<std::string, std::string>> &source) {
  Graph result;
  for (const auto &entry : source) {
    if (!result.contains(entry.first)) {
      result[entry.first] = {};
    }
    if (!result.contains(entry.second)) {
      result[entry.second] = {};
    }
    result.at(entry.first).push_back(entry.second);
    result.at(entry.second).push_back(entry.first);
  }
  return result;
}

std::vector<std::vector<std::string>>
getNInnerConnected(int currentLevel, int n, const Graph &graph,
                   const std::vector<std::vector<std::string>> &clusters) {
  std::set<std::vector<std::string>> newClusters;
  if (currentLevel == 1) {
    for (const auto &entry : graph) {
      newClusters.insert({entry.first});
    }
  } else {
    for (const auto &cluster : clusters) {
      const std::string &vertex = cluster[0];
      const auto &candidates = graph.at(vertex);

      for (const auto &candidate : candidates) {
        bool innerConnected = true;
        for (const auto &member : cluster) {
          auto memberNeighbors = graph.at(member);
          auto found = std::find(memberNeighbors.begin(), memberNeighbors.end(),
                                 candidate);
          if (found == memberNeighbors.end()) {
            innerConnected = false;
            break;
          }
        }
        if (innerConnected) {
          std::vector<std::string> copy(cluster);
          copy.push_back(candidate);
          std::sort (copy.begin(), copy.end());
          newClusters.insert(copy);
        }
      }
    }
  }

  if (currentLevel == n) {
    return std::vector<std::vector<std::string>>(newClusters.begin(), newClusters.end());
  }

  if (newClusters.empty()) {
    return clusters;
  }

  return getNInnerConnected(currentLevel + 1, n, graph,
                            std::vector<std::vector<std::string>>(
                                newClusters.begin(), newClusters.end()));
}

int getConnectedWithT(std::vector<std::vector<std::string>> &clusters) {
  auto filtered =
      clusters | std::views::filter([](const auto &vec) {
        return std::any_of(vec.begin(), vec.end(), [](const auto &elem) {
          return elem.starts_with("t");
        });
      });

  return std::ranges::distance(filtered);
}

int silver() {
  auto input = readInput();
  Graph graph = createGraph(input);
  auto clusters = getNInnerConnected(1, 3, graph, {});
  return getConnectedWithT(clusters);
}

void gold() {
  auto input = readInput();
  Graph graph = createGraph(input);
  auto clusters =
      getNInnerConnected(1, std::numeric_limits<int>().max(), graph, {});

  auto vec = clusters[0];
  std::sort(vec.begin(), vec.end());
  for (const auto &v : vec) {
    std::cout << v << ",";
  }
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: ";
  gold();
}
