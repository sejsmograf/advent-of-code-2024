#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Robot {
  int xPos;
  int yPos;
  int xV;
  int yV;
};

std::vector<Robot> readInput() {
  std::vector<Robot> robots;
  std::ifstream inputStream("../inputs/14.txt");
  std::string line;
  while (std::getline(inputStream, line)) {
    int xPos, yPos, xV, yV;
    std::sscanf(line.c_str(), "p=%d,%d v=%d,%d", &xPos, &yPos, &xV, &yV);
    robots.push_back({xPos, yPos, xV, yV});
  }

  inputStream.close();
  return robots;
}

void printArea(int ysize, int xsize, const std::vector<Robot> &robots) {
  std::vector<std::string> area;
  for (int row = 0; row < ysize; row++) {
    area.push_back("");
    std::cout << "\n";
    for (int col = 0; col < xsize; col++) {
      area[row].append(" ");
    }
  }

  for (const auto &r : robots) {
    area[r.yPos][r.xPos] = '#';
  }

  for (int row = 0; row < ysize; row++) {
    std::cout << area[row];
    std::cout << "\n";
  }
}

unsigned long getCountInQuarter(int ysize, int xsize,
                                std::vector<Robot> &robots) {
  int yHalf = ysize / 2;
  int xHalf = xsize / 2;

  int topL = 0;
  int topR = 0;
  int botL = 0;
  int botR = 0;

  for (const auto &r : robots) {
    if (r.xPos < xHalf && r.yPos < yHalf) {
      topL++;
    } else if (r.xPos > xHalf && r.yPos < yHalf) {
      topR++;
    } else if (r.xPos < xHalf && r.yPos > yHalf) {
      botL++;
    } else if (r.xPos > xHalf && r.yPos > yHalf) {
      botR++;
    }
  }

  return topL * topR * botL * botR;
}

bool detectVerticalLine(const std::vector<Robot> &robots) {
  const int REQUIRED_LENGTH = 5;
  for (const auto r : robots) {
    int requiredX = r.xPos;
    int requiredY = r.yPos;

    bool lineFound = true;
    for (int i = 0; i < REQUIRED_LENGTH; i++) {
      requiredY += 1;
      auto found = std::any_of(
          robots.begin(), robots.end(), [requiredX, requiredY](Robot r) {
            return r.xPos == requiredX && r.yPos == requiredY;
          });
      if(!found) {
        lineFound = false;
        break;
      }
    }
    if(lineFound) {
      return true;
    }
  }
  return false;
}

void simulateMove(int ysize, int xsize, std::vector<Robot> &robots) {
  for (auto &r : robots) {
    int newXPos = (r.xPos + r.xV) % xsize;
    int newYPos = (r.yPos + r.yV) % ysize;
    r.xPos = newXPos >= 0 ? newXPos : xsize + newXPos;
    r.yPos = newYPos >= 0 ? newYPos : ysize + newYPos;
  }
}

unsigned long silver() {
  auto input = readInput();
  const int YSIZE = 103;
  const int XSIZE = 101;
  const int ITERATIONS = 100;

  for (int i = 0; i < ITERATIONS; i++) {
    simulateMove(YSIZE, XSIZE, input);
  }

  return getCountInQuarter(YSIZE, XSIZE, input);
}

unsigned long long gold() {
  auto input = readInput();
  const int YSIZE = 103;
  const int XSIZE = 101;
  const int ITERATIONS = 100;

  char ch;
  int secondCount = 0;
  while (true) {
    secondCount++;
    simulateMove(YSIZE, XSIZE, input);
    if (detectVerticalLine(input)) {
      printArea(YSIZE, XSIZE, input);
      std::cout << "Seconds elapsed: " << secondCount;
      getchar();
    };
  }

  return getCountInQuarter(YSIZE, XSIZE, input);
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
