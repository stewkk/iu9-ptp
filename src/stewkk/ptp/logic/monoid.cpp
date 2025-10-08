#include <stewkk/ptp/logic/monoid.hpp>

namespace stewkk::ptp {

Transformation RightComposition(const Transformation& lhs, const Transformation& rhs) {
  Transformation result;
  result.reserve(lhs.size());
  for (auto lhs_mapping : lhs) {
    result.push_back(rhs[lhs_mapping]);
  }
  return result;
}

Transformation LeftComposition(const Transformation& lhs, const Transformation& rhs) {
  return RightComposition(rhs, lhs);
}

std::string RightWordComposition(const MonoidElement& lhs, const MonoidElement& rhs) {
  return lhs.word + rhs.word;
}

std::string LeftWordComposition(const MonoidElement& lhs, const MonoidElement& rhs) {
  return RightWordComposition(rhs, lhs);
}

CayleyGraphBuilder::CayleyGraphBuilder(const LetterToTransformation& letter_transformations,
                             CompositionStrategy composition_strategy,
                             WordCompositionStrategy word_composition_strategy)
    : composition_strategy_(std::move(composition_strategy)),
      word_composition_strategy_(std::move(word_composition_strategy)) {
  monoid_elements_.reserve(letter_transformations.size());
  for (const auto& [letter, transformation] : letter_transformations) {
    auto [_, is_inserted]
        = monoid_transformations_.emplace(transformation, monoid_elements_.size());
    if (!is_inserted) {
      continue;
    }
    monoid_elements_.push_back(MonoidElement{std::string{letter}, transformation, {}});
  }
}

void CayleyGraphBuilder::AddComposition(size_t element_index, size_t letter_index) {
  auto& lhs = monoid_elements_[element_index];
  auto& rhs = monoid_elements_[letter_index];

  auto composition = composition_strategy_(lhs.transformation, rhs.transformation);
  auto [it, is_inserted] = monoid_transformations_.emplace(composition, monoid_elements_.size());
  lhs.transitions.push_back(it->second);
  if (is_inserted) {
    monoid_elements_.push_back(
        MonoidElement{word_composition_strategy_(lhs, rhs), std::move(composition), {}});
  }
}

CayleyGraph CayleyGraphBuilder::Build() {
    auto letters_count = monoid_elements_.size();
    for (size_t i = 0; i < monoid_elements_.size(); i++) {
        for (int j = 0; j < letters_count; j++) {
          AddComposition(i, j);
        }
    }

    return std::move(monoid_elements_);
}

}  // stewkk::ptp
