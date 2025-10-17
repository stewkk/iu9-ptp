#pragma once

#include <istream>

#include <stewkk/ptp/models/dto.hpp>

namespace stewkk::ptp {

InputDTO ProcessInput(std::istream& input);

}  // namespace stewkk::ptp
