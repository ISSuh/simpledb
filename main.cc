/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <iostream>
#include <string>

#include "skip_list.h"

int32_t main() {
  ssl::SkipList<int32_t, std::string> skip_list;

  skip_list.Update(5, "5");
  skip_list.Print();

  skip_list.Update(10, "10");
  skip_list.Print();

  skip_list.Update(12, "12");
  skip_list.Print();

  return 0;
}


