/**
 *
 *  Copyright:  Copyright (c) 2023, ISSuh
 *
 */

#include <benchmark/benchmark.h>

#include <sdb/skip_list.h>

static void Create(benchmark::State& state) {
  for (auto _ : state) {
    sdb::SkipList<int32_t, std::string>  list(100);
  }
}

static void Update(benchmark::State& state) {
  sdb::SkipList<int32_t, std::string>  list(32);
  int32_t key = 100;
  std::string value = std::to_string(key);
  for (auto _ : state) {
      list.Update(key, value);
  }
}

BENCHMARK(Create)->Unit(benchmark::kMillisecond);
BENCHMARK(Update)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
