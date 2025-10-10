#pragma once

#include <functional>
#include <map>

#include <stewkk/ptp/models/dto.hpp>
#include <stewkk/ptp/models/monoid.hpp>

namespace stewkk::ptp {

LetterToTransformation ToLetterTransformations(InputDTO input);

using CompositionStrategy
    = std::function<Transformation(const Transformation& lhs, const Transformation& rhs)>;
using WordCompositionStrategy
    = std::function<std::string(const MonoidElement& lhs, const MonoidElement& rhs)>;

Transformation RightComposition(const Transformation& lhs, const Transformation& rhs);
Transformation LeftComposition(const Transformation& lhs, const Transformation& rhs);
std::string RightWordComposition(const MonoidElement& lhs, const MonoidElement& rhs);
std::string LeftWordComposition(const MonoidElement& lhs, const MonoidElement& rhs);

class CayleyGraphBuilder {
public:
  CayleyGraphBuilder(const LetterToTransformation& letter_transformations,
                     CompositionStrategy composition_strategy = RightComposition,
                     WordCompositionStrategy word_composition_strategy = RightWordComposition);
  CayleyGraph Build();

private:
  void AddComposition(size_t element_index, size_t letter_index);

private:
  CayleyGraph monoid_elements_;
  std::map<Transformation, size_t> monoid_transformations_;
  CompositionStrategy composition_strategy_;
  WordCompositionStrategy word_composition_strategy_;
};

}  // namespace stewkk::ptp
