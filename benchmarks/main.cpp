#include <benchmark/benchmark.h>

#include <stewkk/ptp/logic/strong_connectivity.hpp>

#include <random>

namespace stewkk::ptp {

namespace {

LetterToTransformation GenerateRandomTransformations(std::size_t size, std::size_t alphabet_size) {
  static std::mt19937 generator(size);
  std::uniform_int_distribution<size_t> distribution(0,
                                                   size-1);
  LetterToTransformation letter_transformations;
  for (std::size_t i = 0; i < alphabet_size; ++i) {
    Transformation transformation;
    transformation.reserve(size);
    for (std::size_t j = 0; j < size; ++j) {
      transformation.push_back(distribution(generator));
    }
    letter_transformations['a'+i] = std::move(transformation);
  }
  return letter_transformations;
}

}  // namespace

template <std::size_t AlphabetSize> static void BM_IdealsByM(benchmark::State& state) {
  std::size_t size = state.range(0);

  auto letter_transformations = GenerateRandomTransformations(size, AlphabetSize);

  for (auto _ : state) {
    auto left_graph
        = CayleyGraphBuilder(letter_transformations, LeftComposition, LeftWordComposition).Build();
    auto right_graph = CayleyGraphBuilder(letter_transformations).Build();

    auto left_condensation_graph = CondensationGraphBuilder(left_graph).Build();
    auto right_condensation_graph = CondensationGraphBuilder(right_graph).Build();

    auto left_ideals = IdealsBuilder(left_graph, left_condensation_graph).Build();
    auto right_ideals = IdealsBuilder(right_graph, right_condensation_graph).Build();

    auto ideals = BuildIdeals(IndicesToWords(left_graph, left_ideals),
                              IndicesToWords(right_graph, right_ideals));
    benchmark::DoNotOptimize(ideals);
  }
}

BENCHMARK(BM_IdealsByM<10>)->DenseRange(2, 4);
BENCHMARK(BM_IdealsByM<5>)->DenseRange(2, 4);
BENCHMARK(BM_IdealsByM<2>)->DenseRange(2, 4);

template <size_t AlphabetSize> static void BM_CondensationByM(benchmark::State& state) {
  std::size_t size = state.range(0);

  auto letter_transformations = GenerateRandomTransformations(size, AlphabetSize);

  for (auto _ : state) {
    auto left_graph
        = CayleyGraphBuilder(letter_transformations, LeftComposition, LeftWordComposition).Build();
    auto right_graph = CayleyGraphBuilder(letter_transformations).Build();

    auto left_condensation_graph = CondensationGraphBuilder(left_graph).Build();
    auto right_condensation_graph = CondensationGraphBuilder(right_graph).Build();

    benchmark::DoNotOptimize(left_condensation_graph);
    benchmark::DoNotOptimize(right_condensation_graph);
  }
}

BENCHMARK(BM_CondensationByM<10>)->DenseRange(2, 8);
BENCHMARK(BM_CondensationByM<5>)->DenseRange(2, 8);
BENCHMARK(BM_CondensationByM<2>)->DenseRange(2, 8);

}  // namespace stewkk::ptp

BENCHMARK_MAIN();
