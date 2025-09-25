#include <stewkk/ptp/logic/monoid.hpp>

namespace stewkk::ptp {

namespace {

Transformation Composition(const Transformation& lhs, const Transformation& rhs) {
  Transformation result;
  result.reserve(lhs.size());
  for (auto lhs_mapping : lhs) {
    result.push_back(rhs[lhs_mapping]);
  }
  return result;
}

} // namespace

MonoidBuilder::MonoidBuilder(const WordToTransformation& letter_transformations)
    : letter_transformations_(letter_transformations) {

    for (const auto& [letter, transformation] : letter_transformations) {
        monoid_transformations_.insert(transformation);
    }

    monoid_elements_.reserve(letter_transformations.size());
    for (const auto& [letter, transformation] : letter_transformations) {
        monoid_elements_.push_back(std::make_pair(letter, transformation));
    }
}

void MonoidBuilder::AddComposition(const std::pair<Word, Transformation>& lhs, const std::pair<Word, Transformation>& rhs) {
  const auto& [lhs_word, lhs_transformation] = lhs;
  const auto& [rhs_word, rhs_transformation] = rhs;

  auto composition = Composition(lhs_transformation, rhs_transformation);
  auto [_, is_inserted] = monoid_transformations_.insert(composition);
  if (is_inserted) {
    monoid_elements_.push_back(std::make_pair(lhs_word + rhs_word, std::move(composition)));
  }
}

WordToTransformation MonoidBuilder::Build() {
    for (size_t i = 0; i < monoid_elements_.size(); i++) {
      AddComposition(monoid_elements_[i], monoid_elements_[i]);
        for (int j = i-1; j >= 0; j--) {
          AddComposition(monoid_elements_[j], monoid_elements_[i]);
          AddComposition(monoid_elements_[i], monoid_elements_[j]);
        }
    }

    WordToTransformation result;
    for (auto& [word, transformation] : monoid_elements_) {
        result[std::move(word)] = std::move(transformation);
    }
    return result;
}

}  // stewkk::ptp
