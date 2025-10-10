#pragma once

#include <map>
#include <string>
#include <vector>

namespace stewkk::ptp {

using Word = std::string;
using Transformation = std::vector<size_t>;
using LetterToTransformation = std::map<char, Transformation>;
using ElementIndex = size_t;

struct MonoidElement {
  std::string word;
  Transformation transformation;
  std::vector<ElementIndex> transitions;

  bool operator<=>(const MonoidElement& other) const = default;
};

using CayleyGraph = std::vector<MonoidElement>;

}  // namespace stewkk::ptp
