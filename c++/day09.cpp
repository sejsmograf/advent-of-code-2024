#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <vector>

using Pos = std::pair<int, int>;

std::string readInput() {
  std::ifstream inputStream("../inputs/09.txt");
  std::string line;
  std::getline(inputStream, line);
  inputStream.close();
  return line;
}

std::vector<int> getFilesystem(const std::string &input) {
  std::vector<int> filesystem;

  for (int i = 0; i < input.size(); i++) {
    int count = input[i] - '0';
    int repr;
    if (i % 2 == 0) {
      repr = i / 2;
    } else {
      repr = -1;
    }
    for (int j = 0; j < count; j++) {
      filesystem.push_back(repr);
    }
  }
  return filesystem;
}

std::vector<int> fragmentateFileSystem(const std::vector<int> &filesystem) {
  std::vector<int> result(filesystem);

  int left = 0;
  int right = result.size() - 1;
  while (left < right) {
    int atLeft = result[left];
    int atRight = result[right];
    if (atLeft != -1) {
      left++;
      continue;
    }
    if (atRight == -1) {
      right--;
      continue;
    }
    result[left] = atRight;
    result[right] = -1;
    left++;
    right--;
  }

  return result;
}

struct MemoryEntry {
  bool isEmpty;
  int fileIndex;
  int size;
};

std::list<MemoryEntry> getMemoryEntries(const std::string &input) {
  std::list<MemoryEntry> files;

  for (int i = 0; i < input.size(); i++) {
    bool isEntryEmpty;
    int size = input[i] - '0';
    if (i % 2 == 0) {
      int fileId = i / 2;
      isEntryEmpty = false;
      files.push_back({isEntryEmpty, fileId, size});
    } else {
      isEntryEmpty = true;
      files.push_back({isEntryEmpty, -1, size});
    }
  }

  return files;
}

std::list<MemoryEntry> fragmentateFileSystemGold(const std::string &input) {
  std::list<MemoryEntry> memory = getMemoryEntries(input);
  std::set<int> moved;

  for (auto it = memory.rbegin(); it!= memory.rend(); ++it) {
    MemoryEntry &fileEntry = *it;

    if (!fileEntry.isEmpty && !moved.contains(fileEntry.fileIndex)) {
      moved.insert(fileEntry.fileIndex);
      for (auto emptyit = memory.begin(); emptyit != it.base(); ++emptyit) {
        MemoryEntry &emptyEntry = *emptyit;

        if (emptyEntry.isEmpty) {
          if (emptyEntry.size >= fileEntry.size) {
            int memoryLeft = emptyEntry.size - fileEntry.size;
            emptyEntry.size = memoryLeft;
            memory.insert(emptyit, fileEntry);
            fileEntry.isEmpty = true;
            fileEntry.fileIndex = -1;
            break;
          }
        }
      }
    }
  }

  return memory;
}

long calculateChecksum(const std::vector<int> &fragmentatedFilesystem) {
  unsigned long long sum = 0;

  for (int i = 0; i < fragmentatedFilesystem.size(); i++) {
    if (fragmentatedFilesystem[i] == -1) {
      return sum;
    }
    sum += i * fragmentatedFilesystem[i];
  }
  return sum;
}

long calculateChecksum(const std::list<MemoryEntry> &memory) {
  std::vector<int> memoryRepr;

  for (auto entry : memory) {
    if (!entry.isEmpty) {
      for (int i = 0; i < entry.size; i++) {
        memoryRepr.push_back(entry.fileIndex);
      }
    } else {
      for (int i = 0; i < entry.size; i++) {
        memoryRepr.push_back(0);
      }
    }
  }

  unsigned long long sum = 0;
  for (int i = 0; i < memoryRepr.size(); i++) {
    sum += i * memoryRepr[i];
  }

  return sum;
}

unsigned long long silver() {
  auto input = readInput();
  auto filesystem = getFilesystem(input);
  auto sejsmo = fragmentateFileSystem(filesystem);
  return calculateChecksum(sejsmo);
}

unsigned long long gold() {
  auto input = readInput();
  auto sejsmo = fragmentateFileSystemGold(input);

  return calculateChecksum(sejsmo);
}

int main() {
  std::cout << "silver: " << silver() << "\n";
  std::cout << "gold: " << gold() << "\n";
}
