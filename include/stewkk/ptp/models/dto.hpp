#pragma once

#include <vector>

#include <stewkk/ptp/models/transformation.hpp>

namespace stewkk::ptp {

using WordDTO = Word;
using TransformationDTO = Transformation;
using WordToTransformationDTO = WordToTransformation;

struct InputDTO {
  std::vector<WordDTO> words;
  WordToTransformationDTO transformations;
};

} // stewkk::ptp
