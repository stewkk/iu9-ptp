#pragma once

#include <vector>

#include <stewkk/ptp/models/monoid.hpp>

namespace stewkk::ptp {

using StronglyConnectedComponents = std::vector<std::vector<ElementIndex>>;

struct CondensationGraph {
  std::vector<size_t> element_to_component;
  std::vector<std::vector<size_t>> transitions;
};

} // stewkk::ptp
