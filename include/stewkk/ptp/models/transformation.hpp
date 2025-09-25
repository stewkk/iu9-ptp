#pragma once

#include <string>
#include <vector>
#include <map>

namespace stewkk::ptp {

using Word = std::string;
using Transformation = std::vector<size_t>;
using WordToTransformation = std::map<Word, Transformation>;

} // stewkk::ptp
