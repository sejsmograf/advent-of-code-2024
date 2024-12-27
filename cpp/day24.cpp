#include <algorithm>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

using Wires = std::map<std::string, bool>;
using Gate = std::pair<std::pair<std::string, std::string>, char>;
using Gates = std::map<std::string, Gate>;
using ul = unsigned long long;

std::pair<std::pair<std::string, std::string>, std::string>
getWiresFromGate(const std::string &gate) {
  size_t arrowPos = gate.find("->");
  std::string source1 = gate.substr(0, 3);
  std::string source2 = gate.substr(arrowPos - 4, 3);
  std::string dest = gate.substr(arrowPos + 3);
  return {{source1, source2}, dest};
}

char getOperationFromGate(const std::string &gate) {
  std::string operationString = gate.substr(4, 3);
  if (operationString == "AND") {
    return 0;
  } else if (operationString == "XOR") {
    return 1;
  } else if (operationString == "OR ") {
    return 2;
  }
  throw std::invalid_argument("Invalid operation for gate: " + gate);
}

std::string toString(char operation) {
  if (operation == 0) {
    return "AND";
  } else if (operation == 1) {
    return "XOR";
  } else if (operation == 2) {
    return "OR ";
  }
  throw std::invalid_argument("Invalid operation");
}

std::pair<Wires, Gates> readInput() {
  Wires wires;
  Gates gates;
  std::ifstream is("../inputs/24.txt");
  std::string line;

  bool gatesStarted = false;
  while (std::getline(is, line)) {
    if (line.empty())
      continue;

    size_t colonPos = line.find(':');
    size_t arrowPos = line.find("->");

    if (colonPos != std::string::npos) {
      std::string var = line.substr(0, colonPos);
      int value = line[colonPos + 2] - '0'; // Convert char to int
      wires[var] = (bool)value;
    } else if (arrowPos != std::string::npos) {
      auto [sources, dest] = getWiresFromGate(line);
      char operation = getOperationFromGate(line);
      gates[dest] = {sources, operation};
    }
  }

  is.close();
  return {wires, gates};
}

void evalGate(const Gates &gates, const std::string &gateKey, Wires &wires) {
  if (wires.contains(gateKey)) {
    return;
  }

  const auto &[sources, op] = gates.at(gateKey);

  evalGate(gates, sources.first, wires);
  evalGate(gates, sources.second, wires);

  bool a = wires.at(sources.first);
  bool b = wires.at(sources.second);
  bool result;
  switch (op) {
  case 0:
    result = a & b;
    break;
  case 1:
    result = a ^ b;
    break;
  case 2:
    result = a | b;
    break;
  }

  wires[gateKey] = result;
}

ul sumWiresStartingWith(const Wires &wires, char ch) {
  std::vector<std::pair<std::string, bool>> startWith;
  for (const auto &wire : wires) {
    if (wire.first.starts_with(ch)) {
      startWith.push_back(wire);
    }
  }
  std::sort(startWith.begin(), startWith.end(),
            [](const auto &a, const auto &b) { return a.first > b.first; });

  ul acc = 0;
  for (const auto &entry : startWith) {
    acc = (acc << 1) | (int)entry.second;
  }

  return acc;
}

std::string gateInputsToString(int depth, const std::string &gateKey,
                               Gates &gates) {
  if (depth == 0 || !gates.contains(gateKey)) {
    return gateKey;
  }

  std::string result;
  const auto &entry = gates.at(gateKey);

  return "(" + gateInputsToString(depth - 1, entry.first.first, gates) + " " +
         toString(entry.second) + " " +
         gateInputsToString(depth - 1, entry.first.second, gates) + ")";
}

auto findGateWith(const std::string &sourceOne, const std::string &sourceTwo,
                  char op, const Gates &gates) {
  auto found = std::find_if(gates.begin(), gates.end(),
                            [&sourceOne, &sourceTwo, op](const auto &gate) {
                              return (gate.second.first.first == sourceOne ||
                                      gate.second.first.first == sourceTwo) &&
                                     (gate.second.first.second == sourceOne ||
                                      gate.second.first.second == sourceTwo) &&
                                     gate.second.second == op;
                            });

  return found;
}

auto findGateWith(const std::string &sourceOne, char op, const Gates &gates) {
  auto found = std::find_if(gates.begin(), gates.end(),
                            [&sourceOne, op](const auto &gate) {
                              return (gate.second.first.first == sourceOne ||
                                      gate.second.first.second == sourceOne) &&
                                     gate.second.second == op;
                            });

  return found;
}

void checkResultGate(const std::string &gateKey, Gates &gates,
                     std::vector<std::string> &swaps) {
  std::cout << "CHECKING GATE: " << gateKey << "\n";
  if (gateKey == "z00" || gateKey == "z45") {
    return;
  }

  const Gate &gate = gates.at(gateKey);
  if(gate.second == 1) {
    return;
  }
  const std::string suffix = gateKey.substr(1);

  const auto xyInput = findGateWith("x" + suffix, "y" + suffix, 1, gates);
  if (xyInput == gates.end()) {
    throw std::invalid_argument("xyInput must be found for each gate");
  }
  const std::string xyInputName = xyInput->first;

  const auto shouldBe = findGateWith(xyInputName, 1, gates);
  if (shouldBe == gates.end()) {
    throw std::invalid_argument("xyInput must be found for each gate");
  }
  const std::string shouldBeName = shouldBe->first;

  if (shouldBeName != gateKey) {
    std::cout << "SWAPPING " << gateKey << " | " << shouldBeName;
    gates[shouldBeName] = gate;
    gates[gateKey] = shouldBe->second;
    swaps.push_back(gateKey);
    swaps.push_back(shouldBeName);
  }
}

ul silver() {
  auto input = readInput();
  auto wires = input.first;
  auto gates = input.second;

  for (const auto &gate : gates) {
    evalGate(gates, gate.first, wires);
  }

  return sumWiresStartingWith(wires, 'z');
}

void gold() {
  auto input = readInput();
  auto wires = input.first;
  auto gates = input.second;

  ul target =
      sumWiresStartingWith(wires, 'x') + sumWiresStartingWith(wires, 'y');
  std::string targetBits = std::bitset<64>(target).to_string();
  std::cout << "\n" << targetBits << "\n";

  for (const auto &entry : gates) {
    std::cout << entry.first << " = "
              << gateInputsToString(2, entry.first, gates) << "\n\n\n";
  }

  ul actual = sumWiresStartingWith(wires, 'z');
  std::string actualBits = std::bitset<64>(actual).to_string();
  std::cout << actualBits << "\n";

  auto resultGates = gates | std::ranges::views::filter([](const auto &wire) {
                       return wire.first.starts_with("z");
                     });

  std::vector<std::string> result;
  for (const auto gate : resultGates) {
    checkResultGate(gate.first, gates, result);
  }
  result.push_back("nnr");
  result.push_back("rqf");
  std::sort(result.begin(), result.end());
  for(const auto& sejsmo: result) {
    std::cout << sejsmo << ",";
  }
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: ";
  gold();
}
