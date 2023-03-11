/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <iostream>
#include <string>

#include "skip_list.h"

int32_t main() {
  ssl::SkipList<int32_t, std::string> skip_list(5);

  skip_list.Update(5, "5");
  skip_list.Print();

  skip_list.Update(10, "10");
  skip_list.Print();

  skip_list.Update(12, "12");
  skip_list.Print();

  skip_list.Update(12, "31");
  skip_list.Print();

  bool res = skip_list.Remove(5);
  if (res) {
    std::cout << "remove success" << std::endl;
  } else {
    std::cout << "remove fail" << std::endl;
  }
  skip_list.Print();

  res = skip_list.Remove(88);
  if (res) {
    std::cout << "remove success" << std::endl;
  } else {
    std::cout << "remove fail" << std::endl;
  }
  skip_list.Print();

  res = skip_list.Remove(10);
  if (res) {
    std::cout << "remove success" << std::endl;
  } else {
    std::cout << "remove fail" << std::endl;
  }
  skip_list.Print();

  skip_list.Update(123, "123");
  skip_list.Print();

  return 0;
}


