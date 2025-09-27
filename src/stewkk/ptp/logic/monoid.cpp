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

MonoidBuilder::MonoidBuilder(const LetterToTransformation& letter_transformations) {
    monoid_elements_.reserve(letter_transformations.size());
    for (const auto& [letter, transformation] : letter_transformations) {
      auto [_, is_inserted] = monoid_transformations_.emplace(transformation, monoid_elements_.size());
      if (!is_inserted) {
        continue;
      }
      monoid_elements_.push_back(MonoidElement{std::string{letter}, transformation, {}});
    }
}

void MonoidBuilder::AddComposition(size_t element_index, size_t letter_index) {
  auto& lhs = monoid_elements_[element_index];
  auto& rhs = monoid_elements_[letter_index];

  auto composition = Composition(lhs.transformation, rhs.transformation);
  auto [it, is_inserted] = monoid_transformations_.emplace(composition, monoid_elements_.size());
  lhs.transitions.push_back(it->second);
  if (is_inserted) {
    monoid_elements_.push_back(MonoidElement{lhs.word + rhs.word, std::move(composition), {}});
  }
}

TransformationMonoid MonoidBuilder::Build() {
    auto letters_count = monoid_elements_.size();
    for (size_t i = 0; i < monoid_elements_.size(); i++) {
        for (int j = 0; j < letters_count; j++) {
          AddComposition(i, j);
        }
    }

    return std::move(monoid_elements_);
}

}  // stewkk::ptp
