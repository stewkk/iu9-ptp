#pragma once

#include <iostream>

#include <stewkk/ptp/models/monoid.hpp>

namespace stewkk::ptp {

void VisualizeDot(std::ostream& out, const CayleyGraph& monoid);

}  // stewkk::ptp
