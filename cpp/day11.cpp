#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>

std::list<unsigned long long> readInput() {
  std::list<unsigned long long> result;
  std::ifstream inputStream("../inputs/11.txt");
  std::string line;
  long rowIndex = 0;

  while (std::getline(inputStream, line)) {
    std::stringstream ss(line);
    unsigned long long number;
    while (ss >> number) {
      result.push_back(number);
    }
  }

  inputStream.close();
  return result;
}

unsigned long long getDigitCount(unsigned long long n) {
  if (n == 0) {
    return 1;
  }
  return (int)std::log10(n) + 1;
}

std::pair<unsigned long long, unsigned long long>
splitNumberInHalf(unsigned long long n, unsigned long long digits) {
  unsigned long long half = digits / 2;
  unsigned long long split = std::pow(10, half);

  return {n / split, n % split};
}

void singleBlink(std::list<unsigned long long> &stones) {
  for (auto it = stones.begin(); it != stones.end(); ++it) {
    unsigned long long stone = *it;
    if (stone == 0) {
      *it += 1;
    } else if (unsigned long long digits = getDigitCount(stone);
               digits % 2 == 0) {
      std::pair<unsigned long long, unsigned long long> split =
          splitNumberInHalf(stone, digits);
      stones.insert(it, split.first);
      *it = split.second;
    } else {
      *it *= 2024;
    }
  }
}

unsigned long long simulateBlinkRecursive(unsigned long long stone,
                                          int blinksRemaining) {
  unsigned long long count = 0;
  if (blinksRemaining == 0) {
    return 1;
  }

  if (stone == 0) {
    count += simulateBlinkRecursive(1, blinksRemaining - 1);
  } else if (unsigned long long digits = getDigitCount(stone);
             digits % 2 == 0) {
    std::pair<unsigned long long, unsigned long long> split =
        splitNumberInHalf(stone, digits);
    count += simulateBlinkRecursive(split.first, blinksRemaining - 1);
    count += simulateBlinkRecursive(split.second, blinksRemaining - 1);
  } else {
    count += simulateBlinkRecursive(stone * 2024, blinksRemaining - 1);
  }

  return count;
}

unsigned long long simulateBlinks(std::list<unsigned long long> stones,
                                  long blinks) {
  unsigned long long sum = 0;

  std::unordered_map<unsigned long long, unsigned long long> memo;
  int counter = 0;
  for (auto it = stones.begin(); it != stones.end(); ++it) {
    if (counter % 1000 == 0) {
      std::cout << counter << " / " << stones.size() << "\n";
    }
    unsigned long long stone = *it;
    if (memo.contains(stone)) {
      sum += memo[stone];
    } else {
      unsigned long afterBlinks = simulateBlinkRecursive(stone, blinks);
      sum += afterBlinks;
      memo.insert({stone, afterBlinks});
    }
    counter++;
  }

  return sum;
}

unsigned long long silver() {
  auto input = readInput();
  for (unsigned long long i = 0; i < 10; i++) {
    singleBlink(input);
  }

  return simulateBlinks(input, 15);
}

unsigned long long gold() {
  auto input = readInput();
  for (unsigned long long i = 0; i < 43; i++) {
    std::cout << "Blink " << i << " / 43\n";
    singleBlink(input);
  }

  return simulateBlinks(input, 32);
  return 0;
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
