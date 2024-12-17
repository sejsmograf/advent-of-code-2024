#include <array>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using ul = unsigned long long;
using Register = std::array<ul, 3>;
using Instr = std::pair<ul, ul>;

std::pair<Register, std::vector<Instr>> readInput() {
  Register reg;
  std::vector<Instr> instructions;
  std::ifstream is("../inputs/17.txt");
  std::string line;

  int regIndex = 0;
  while (std::getline(is, line)) {
    if (line.empty()) {
      std::getline(is, line);
      std::stringstream ss(line);
      int instr, operand;
      char sep;
      ss >> line;

      while (ss >> instr >> sep >> operand) {
        Instr instruction = {instr, operand};
        instructions.push_back(instruction);
        ss >> sep;
      }
    } else {
      int regValue;
      std::sscanf(line.c_str(), "Register %*c: %d", &regValue);
      reg[regIndex] = regValue;
      regIndex++;
    }
  }

  is.close();
  return {reg, instructions};
}

ul getComboOperandValue(const Register &reg, ul operand) {
  if (operand >= 0 && operand <= 3) {
    return operand;
  }
  return reg[operand % 4];
}

void adv(Register &reg, const Instr &instruction) {
  ul numerator = reg[0];
  ul operand = getComboOperandValue(reg, instruction.second);
  ul denominator = numerator / (std::pow(2, operand));
  reg[0] = denominator;
}

void bxl(Register &reg, const Instr &instruction) {
  ul first = reg[1];
  ul operand = instruction.second;
  reg[1] = first ^ operand;
}

void bst(Register &reg, const Instr &instruction) {
  reg[1] = getComboOperandValue(reg, instruction.second) % 8;
}

bool jnz(Register &reg, const Instr &instruction) { return reg[0] != 0; }

void bxc(Register &reg, const Instr &instruction) {
  ul b = reg[1];
  ul c = reg[2];
  reg[1] = b ^ c;
}

ul out(Register &reg, const Instr &instruction) {
  ul value = getComboOperandValue(reg, instruction.second);
  return value % 8;
}

void bdv(Register &reg, const Instr &instruction) {
  ul numerator = reg[0];
  ul operand = getComboOperandValue(reg, instruction.second);
  ul denominator = numerator / (std::pow(2, operand));
  reg[1] = denominator;
}

void cdv(Register &reg, const Instr &instruction) {
  ul numerator = reg[0];
  ul operand = getComboOperandValue(reg, instruction.second);
  ul denominator = numerator / (std::pow(2, operand));
  reg[2] = denominator;
}

std::vector<ul> performInstructions(Register reg,
                                    const std::vector<Instr> &instructions,
                                    bool optimize,
                                    const std::vector<ul> &desired) {
  std::vector<ul> outputs;
  ul pc = 0;

  while (pc < instructions.size()) {
    const Instr &instr = instructions[pc];
    bool jumped = false;

    switch (instr.first) {
    case 0:
      adv(reg, instr);
      break;
    case 1:
      bxl(reg, instr);
      break;
    case 2:
      bst(reg, instr);
      break;
    case 3:
      if (jnz(reg, instr)) {
        jumped = true;
        pc = instr.second / 2;
      }
      break;
    case 4:
      bxc(reg, instr);
      break;
    case 5:
      outputs.push_back(out(reg, instr));
      if (optimize) {
        int last = outputs.size() - 1;
        if (last >= desired.size() || outputs[last] != desired[last]) {
          return {};
        }
      }
      break;
    case 6:
      bdv(reg, instr);
      break;
    case 7:
      cdv(reg, instr);
      break;
    }
    if (!jumped) {
      pc += 1;
    }
  }

  return outputs;
}

void silver() {
  auto input = readInput();
  auto outputs = performInstructions(input.first, input.second, false, {});
  for (const auto out : outputs) {
    std::cout << out << ",";
  }
}

ul gold() {
  auto input = readInput();
  std::vector<ul> desiredOutput;
  for (const auto &instr : input.second) {
    desiredOutput.push_back(instr.first);
    desiredOutput.push_back(instr.second);
  }

  ul index = 0;

  while (true) {
    if(index % 10000 == 0){
      std::cout << "Checking index: " << index << "\n";
    }

    Register copy(input.first);
    copy[0] = index;

    auto outputs = performInstructions(copy, input.second, true, desiredOutput);
    if (outputs == desiredOutput) {
      break;
    }
    index++;
  }

  return index;
}

int main() {
  std::cout << "silver: ";
  silver();
  std::cout << "\nold: " << gold();
}
