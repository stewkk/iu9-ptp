#pragma once

#include <map>
#include <string>
#include <vector>

namespace stewkk::ptp {

using WordDTO = std::string;
using TransformationDTO = std::map<WordDTO, WordDTO>;
using WordToTransformationDTO = std::map<WordDTO, TransformationDTO>;

struct InputDTO {
  std::vector<WordDTO> words;
  WordToTransformationDTO transformations;
};

}  // namespace stewkk::ptp
