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
