#include <algorithm>
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
                                    const std::vector<Instr> &instructions) {
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

std::vector<int> getThreeBits(int desiredOutput, ul suffix, const Register &reg,
                              const std::vector<Instr> &instructions) {
  std::vector<int> result;
  for (int i = 0; i <= 7; i++) {
    Register copy(reg);
    ul combined = suffix + i;
    copy[0] = combined;
    auto outputs = performInstructions(copy, instructions);
    int createdBits = outputs[0];
    if (createdBits == desiredOutput) {
      result.push_back(i);
    }
  }

  return result;
}

void silver() {
  auto input = readInput();
  auto outputs = performInstructions(input.first, input.second);
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

  std::vector<ul> possibleNumbers = {0};
  for (int i = desiredOutput.size() - 1; i >= 0; i--) {
    int desiredBits = desiredOutput[i];

    std::vector<ul> validSoFar;
    for (ul suffix : possibleNumbers) {
      std::vector<int> validBits =
          getThreeBits(desiredBits, suffix, input.first, input.second);

      for (int valid : validBits) {
        validSoFar.push_back((suffix + valid) << 3);
      }
    }
    possibleNumbers = validSoFar;
  }

  for (int i = 0; i < possibleNumbers.size(); i++) {
    possibleNumbers[i] = possibleNumbers[i] >> 3;
  }

  std::sort(possibleNumbers.begin(), possibleNumbers.end());
  return possibleNumbers[0] << 3;
}

int main() {
  std::cout << "silver: ";
  silver();
  std::cout << "\nold: " << gold();
}
