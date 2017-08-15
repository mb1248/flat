#include "include/flat_set.hpp"
#include "include/flat_map.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  fc::vector_set<int> set;
  fc::vector_map<int,int> map;

  set.insert(5);
  set.insert(3);
  map[4] = 2;
  map[2] = 3;

  for (auto& i : set) {
    std::cout << i << "\n";
  }

  for (auto& i : map) {
    std::cout << i.first << ": " << i.second << "\n";
  }
}
