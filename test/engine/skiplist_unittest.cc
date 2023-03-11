/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <string>
#include <random>
#include <cmath>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sdb/skip_list.h>

size_t GenerateRandom(size_t range_from, size_t range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<size_t> distribution(range_from, range_to);
  return distribution(generator);
}

TEST(Engine, create) {
  sdb::SkipList<int32_t, std::string>  list(100);
  EXPECT_EQ(list.MaxLevel(), 100);
}

TEST(Engine, update) {
  bool res = false;
  std::string value("");
  sdb::SkipList<int32_t, std::string>  list;

  res = list.Find(100, &value);
  EXPECT_FALSE(res);

  list.Update(10, "10");
  res = list.Find(10, &value);
  EXPECT_TRUE(res);
  EXPECT_STREQ(value.c_str(), "10");

  list.Update(10, "1000");
  res = list.Find(10, &value);
  EXPECT_TRUE(res);
  EXPECT_STREQ(value.c_str(), "1000");
}

TEST(Engine, update_large) {
  int32_t num = 10000;
  bool res = false;
  std::string value("");

  int32_t size = static_cast<double>(std::log2(static_cast<double>(num)));
  sdb::SkipList<int32_t, std::string> list(16);

  for (int32_t i = 0 ; i < num ; ++i) {
    int32_t key = i;
    std::string value = std::to_string(key);
    list.Update(key, value);
  }

  EXPECT_EQ(list.Size(), num);
}

TEST(Engine, erase) {
  int32_t num = 10000;
  bool res = false;
  std::string value("");

  int32_t size = static_cast<double>(std::log2(static_cast<double>(num)));
  sdb::SkipList<int32_t, std::string> list(size);

  for (int32_t i = 0 ; i < num ; ++i) {
    int32_t key = i;
    std::string value = std::to_string(key);
    list.Update(key, value);
  }

  int32_t random_key = GenerateRandom(0, num);
  res = list.Find(random_key, &value);
  EXPECT_TRUE(res);
  EXPECT_STREQ(value.c_str(), std::to_string(random_key).c_str());

  value.clear();

  list.Erase(random_key);
  res = list.Find(random_key, &value);
  EXPECT_FALSE(res);
}
