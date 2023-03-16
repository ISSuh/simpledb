/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <benchmark/benchmark.h>

#include <sdb/skip_list.h>
#include <sdb/rand.h>

size_t GenerateRandom(size_t range_from, size_t range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<size_t> distribution(range_from, range_to);
  return distribution(generator);
}

static void Create(benchmark::State& state) {
  for (auto _ : state) {
    sdb::SkipList<int32_t, std::string>  list(100);
  }
}

static void Update(benchmark::State& state) {
  sdb::SkipList<int32_t, int32_t>  list(32);
  for (auto _ : state) {
    for (auto i = 0 ; i < 1000 ; ++i) {
      list.Update(i, i);
    }
  }
}

BENCHMARK(Create)->Unit(benchmark::kMillisecond);
BENCHMARK(Update)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
