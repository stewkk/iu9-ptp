#pragma once

#include <string>
#include <vector>
#include <map>

namespace stewkk::ptp {

using Word = std::string;
using Transformation = std::vector<size_t>;
using LetterToTransformation = std::map<char, Transformation>;

struct MonoidElement {
    std::string word;
    Transformation transformation;
    std::vector<size_t> transitions;
};

using TransformationMonoid = std::vector<MonoidElement>;

} // stewkk::ptp
