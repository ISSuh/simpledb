/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#ifndef SDB_RAND_H_
#define SDB_RAND_H_

#include <random>

namespace sdb {

namespace {
std::random_device rd;
uint64_t s[2] =
  { (uint64_t(rd()) << 32) ^ (rd()),
    (uint64_t(rd()) << 32) ^ (rd()) };

uint64_t curRand;
uint8_t bit = 63;
}  // namespace

uint64_t xorshift128plus() {
  uint64_t x = s[0];
  uint64_t const y = s[1];
  s[0] = y;
  x ^= x << 23;
  s[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
  return s[1] + y;
}

bool CoinFlip() {
  if (bit >= 63) {
    curRand = xorshift128plus();
    bit = 0;
    return curRand & 1;
  } else {
    bit++;
    return curRand & (1 << bit);
  }
}

}  // namespace sdb

#endif  // SDB_RAND_H_
