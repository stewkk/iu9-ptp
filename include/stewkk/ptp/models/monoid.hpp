#pragma once

#include <string>
#include <vector>
#include <map>

namespace stewkk::ptp {

using Word = std::string;
using Transformation = std::vector<size_t>;
using LetterToTransformation = std::map<char, Transformation>;

struct MonoidElement {
    Transformation transformation;
    std::vector<size_t> transitions;
};

using TransformationMonoid = std::map<Word, MonoidElement>;
using LabeledElement = TransformationMonoid::value_type;

} // stewkk::ptp
