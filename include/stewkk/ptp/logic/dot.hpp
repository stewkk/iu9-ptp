#pragma once

#include <iostream>

#include <stewkk/ptp/models/monoid.hpp>

namespace stewkk::ptp {

void VisualizeDot(std::ostream& out, const TransformationMonoid& monoid);

}  // stewkk::ptp
