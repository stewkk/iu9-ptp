#pragma once

#include <string>
#include <map>

namespace stewkk::ptp {

using Word = std::string;
using Transformation = std::map<Word, Word>;
using WordToTransformation = std::map<Word, Transformation>;

} // stewkk::ptp
