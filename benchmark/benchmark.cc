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

static void Rand1(benchmark::State& state) {
  sdb::SkipList<int32_t, int32_t>  list(32);
  for (auto _ : state) {
    auto rand = GenerateRandom(0, 16);
  }
}

static void Rand2(benchmark::State& state) {
  sdb::SkipList<int32_t, int32_t>  list(32);
  for (auto _ : state) {
    auto rand = redisLrand48() % 16;
  }
}

BENCHMARK(Create)->Unit(benchmark::kMillisecond);
BENCHMARK(Update)->Unit(benchmark::kMillisecond);
BENCHMARK(Rand1)->Unit(benchmark::kMillisecond)->Arg(1000);
BENCHMARK(Rand2)->Unit(benchmark::kMillisecond)->Arg(1000);

BENCHMARK_MAIN();
