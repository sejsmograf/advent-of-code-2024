#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>




std::pair<std::vector<std::string>, std::vector<std::string>> readInput() {
  std::vector<std::vector<std::string>> locks;
  std::vector<std::vector<std::string>> keys;
  std::ifstream is("../inputs/25.txt");
  std::string line;

  bool gatesStarted = false;
  while (std::getline(is, line)) {
    std::vector<std::string> input;
    for(int i = 0; i < 7; i++ ){

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

ul silver() {
  auto input = readInput();
  auto wires = input.first;
  auto gates = input.second;

  for (const auto &gate : gates) {
    evalGate(gates, gate.first, wires);
  }

  return sumWiresStartingWith(wires, 'z');
}

ul gold() {
  auto input = readInput();
  auto wires = input.first;
  auto gates = input.second;

  ul target =
      sumWiresStartingWith(wires, 'x') & sumWiresStartingWith(wires, 'y');
  std::string targetBits = std::bitset<16>(target).to_string();
  std::cout << "\n" << targetBits << "\n";

  auto temp = gates["z00"];

  for (const auto &entry : gates) {
    evalGate(gates, entry.first, wires);
  }

  ul actual = sumWiresStartingWith(wires, 'z');
  std::string actualBits = std::bitset<16>(actual).to_string();
  std::cout << actualBits;

  return target;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold();
}
