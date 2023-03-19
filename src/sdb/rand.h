/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#ifndef SDB_RAND_H_
#define SDB_RAND_H_

#include <cstdint>

namespace sdb {

// get main algorithm from leveldb
class Random {
 public:
  explicit Random(uint32_t s)
    : seed_(s & 0x7fffffffu) {
    if (seed_ == 0 || seed_ == 2147483647L) {
      seed_ = 1;
    }
  }

  uint32_t Next() {
    static const uint32_t M = 2147483647L;
    static const uint64_t A = 16807;
    uint64_t product = seed_ * A;

    seed_ = static_cast<uint32_t>((product >> 31) + (product & M));

    if (seed_ > M) {
      seed_ -= M;
    }
    return seed_;
  }

  uint32_t Uniform(int32_t range) { return Next() % range; }

 private:
  uint32_t seed_;
};

}  // namespace sdb

#endif  // SDB_RAND_H_
