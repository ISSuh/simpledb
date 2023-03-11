/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <iostream>
#include <string>

#include "sdb/skip_list.h"

int32_t main() {
  sdb::SkipList<int32_t, std::string> skip_list(5);

  skip_list.Update(5, "5");
  skip_list.Print();

  skip_list.Update(10, "10");
  skip_list.Print();

  skip_list.Update(12, "12");
  skip_list.Print();

  skip_list.Update(12, "31");
  skip_list.Print();

  skip_list.Erase(5);
  skip_list.Print();

  skip_list.Erase(88);
  skip_list.Print();

  skip_list.Update(123, "123");
  skip_list.Print();

  return 0;
}


