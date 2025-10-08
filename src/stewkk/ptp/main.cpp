#include <iostream>
#include <ranges>

#include <stewkk/ptp/logic/input.hpp>
#include <stewkk/ptp/logic/dot.hpp>
#include <stewkk/ptp/logic/strong_connectivity.hpp>
#include <stewkk/ptp/logic/monoid.hpp>

using namespace stewkk::ptp;

std::string FormatIdeal(const std::vector<std::vector<std::string>>& ideals) {
  return ideals | std::ranges::views::transform([](const auto& ideal){
    return ideal | std::ranges::views::join_with(' ') | std::ranges::to<std::string>();
  }) | std::ranges::views::join_with('\n') | std::ranges::to<std::string>();
}

int32_t main() {
  auto input = ProcessInput(std::cin);

  auto letter_transformations = ToLetterTransformations(input);
  auto left_graph = CayleyGraphBuilder(letter_transformations, LeftComposition, LeftWordComposition).Build();
  auto right_graph = CayleyGraphBuilder(letter_transformations).Build();

  auto left_condensation_graph = CondensationGraphBuilder(left_graph).Build();
  auto right_condensation_graph = CondensationGraphBuilder(right_graph).Build();

  auto left_ideals = IdealsBuilder(left_graph, left_condensation_graph).Build();
  auto right_ideals = IdealsBuilder(right_graph, right_condensation_graph).Build();

  auto left_ideals_words = IndicesToWords(left_graph, left_ideals);
  auto right_ideals_words = IndicesToWords(right_graph, right_ideals);
  auto ideals = BuildIdeals(left_ideals_words,
                            right_ideals_words);

  std::cout << "Left ideals:\n" << FormatIdeal(left_ideals_words) << '\n';
  std::cout << "Right ideals:\n" << FormatIdeal(right_ideals_words) << '\n';
  std::cout << "Ideals:\n" << FormatIdeal(right_ideals_words) << '\n';

  return 0;
}
