#pragma once

#include <map>
#include <functional>

#include <stewkk/ptp/models/monoid.hpp>

namespace stewkk::ptp {

using CompositionStrategy = std::function<Transformation(const Transformation& lhs, const Transformation& rhs)>;
using WordCompositionStrategy = std::function<std::string(const MonoidElement& lhs, const MonoidElement& rhs)>;

Transformation RightComposition(const Transformation& lhs, const Transformation& rhs);
Transformation LeftComposition(const Transformation& lhs, const Transformation& rhs);
std::string RightWordComposition(const MonoidElement& lhs, const MonoidElement& rhs);
std::string LeftWordComposition(const MonoidElement& lhs, const MonoidElement& rhs);

class MonoidBuilder {
public:
  MonoidBuilder(const LetterToTransformation& letter_transformations,
                         CompositionStrategy composition_strategy = RightComposition,
                         WordCompositionStrategy word_composition_strategy = RightWordComposition);
  TransformationMonoid Build();

private:
    void AddComposition(size_t element_index, size_t letter_index);

private:
    TransformationMonoid monoid_elements_;
    std::map<Transformation, size_t> monoid_transformations_;
    CompositionStrategy composition_strategy_;
    WordCompositionStrategy word_composition_strategy_;
};

}  // stewkk::ptp
