#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>


std::list<unsigned long long> readInput() {
  std::list<unsigned long long> result;
  std::ifstream inputStream("../inputs/11.txt");
  std::string line;
  unsigned long long rowIndex = 0;

  while (std::getline(inputStream, line)) {
    std::stringstream ss(line);
    unsigned long long number;
    while(ss >> number) {
      result.push_back(number);
    }
  }

  inputStream.close();
  return result;
}

int getDigitCount(unsigned long long n) {
  if(n == 0){
    return 1;
  }
  return (unsigned long long)std::log10(n) + 1;
}

std::pair<unsigned long long, unsigned long long> splitNumberInHalf(unsigned long long n, unsigned long long digits) {
  unsigned long long half = digits / 2;
  unsigned long long split = std::pow(10, half);

  return {n / split, n % split};
}


void singleBlink (std::list<unsigned long long>& stones) {
  for (auto it = stones.begin(); it != stones.end(); ++it){
    unsigned long long stone = *it;
    if(stone == 0) {
      *it += 1;
    }
    else if (unsigned long long digits = getDigitCount(stone); digits % 2 ==0){
      std::pair<unsigned long long, unsigned long long> split = splitNumberInHalf(stone, digits);
      stones.insert(it, split.first);
      *it = split.second;
    }
    else {
      *it *= 2024;
    }
  }
}


unsigned long long simulateBlinks(std::list<unsigned long long> stones, unsigned long long blinks) {
  std::list<unsigned long long> zeroed = {0};
  for(unsigned long long i = 0 ; i < blinks; i++) {
    std::cout << "Blink " << i << "\n";
    std::cout<< stones.size() << "\n\n";
    singleBlink(zeroed);
  }
  int zeroSubstitute = zeroed.size();
  unsigned long long zerosSum = 0;

  for(auto it = stones.begin(); it != stones.end(); ++it) {
    if(*it == 0) {
      zerosSum += zeroSubstitute;
      it = stones.erase(it);
    }
  }


  for(unsigned long long i = 0 ; i < blinks; i++) {
    std::cout << "Blink " << i << "\n";
    std::cout<< stones.size() << "\n\n";
    singleBlink(stones);
  }

  return stones.size() + zerosSum;
}




unsigned long long silver() { 
  auto input = readInput();
  return simulateBlinks(input, 25);
}

unsigned long long gold() {
  auto input = readInput();
  return simulateBlinks(input, 75);
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
