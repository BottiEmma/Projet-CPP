#pragma once

#include <iostream>
#include <vector>

template <typename T>
void print_vec(const std::vector<T> vec) {
  std::cout << "[ (" << vec.size() << ")\n";
  for (const T &it : vec) {
    std::cout << "  ";
    if (std::is_pointer<T>::value) {
      std::cout << *it;
    } else {
      std::cout << it;
    }
    std::cout << "\n";
  }
  std::cout << "]\n";
}
