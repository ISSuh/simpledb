/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <string>
#include <cmath>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sdb/skip_list.h>
#include <sdb/rand.h>

TEST(Engine, create) {
  sdb::SkipList<int32_t, std::string>  list(100);
  EXPECT_EQ(list.MaxLevel(), 100);
}

TEST(Engine, update) {
  bool res = false;
  sdb::SkipList<int32_t, std::string> list;

  std::string value("");
  res = list.Find(100, &value);
  EXPECT_FALSE(res);
  EXPECT_STREQ(value.c_str(), "");

  list.Update(10, "10");
  res = list.Find(10, &value);
  list.Print();
  EXPECT_TRUE(res);
  EXPECT_STREQ(value.c_str(), "10");

  list.Update(10, "1000");
  res = list.Find(10, &value);
  list.Print();
  EXPECT_TRUE(res);
  EXPECT_STREQ(value.c_str(), "1000");
}

TEST(Engine, erase) {
  int32_t num = 10;
  bool res = false;
  std::string value("");

  int32_t size = static_cast<double>(std::log2(static_cast<double>(num)));
  sdb::SkipList<int32_t, std::string> list(size);

  for (int32_t i = 0 ; i <= num ; ++i) {
    int32_t key = i;
    std::string value = std::to_string(key);
    list.Update(key, value);
  }

  sdb::Random rand(0xdeadbeef);
  int32_t random_key = rand.Uniform(num);
  res = list.Find(random_key, &value);
  EXPECT_TRUE(res);
  EXPECT_STREQ(value.c_str(), std::to_string(random_key).c_str());

  list.Print();

  value.clear();

  list.Erase(random_key);
  res = list.Find(random_key, &value);
  EXPECT_FALSE(res);

  list.Print();
}
