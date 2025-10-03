#include <stewkk/ptp/logic/strong_connectivity.hpp>

#include <ranges>

namespace stewkk::ptp {

namespace {

TransposedGraph GetTransposedGraph(const TransformationMonoid& monoid) {
  TransposedGraph result(monoid.size());

  for (auto [vertex_index, vertex] : std::ranges::views::enumerate(monoid)) {
    result[vertex_index].resize(vertex.transitions.size());
  }

  for (auto [start_index, start] : std::ranges::views::enumerate(monoid)) {
    for (auto [letter_index, end_index] : std::ranges::views::enumerate(start.transitions)) {
      result[end_index][letter_index].push_back(start_index);
    }
  }

  return result;
}

}  // namespace

RightIdealsFinder::RightIdealsFinder(const TransformationMonoid& monoid,
                                     const StronglyConnectedComponents& scc)
    : monoid_(monoid), scc_(scc) {}

std::vector<std::vector<ElementIndex>> RightIdealsFinder::FindRightIdeals() {
  element_to_component_.resize(monoid_.size());
  for (auto [component_index, component] : std::ranges::views::enumerate(scc_)) {
    for (auto element : component) {
      element_to_component_[element] = component_index;
    }
  }
  component_to_ideal_.assign(scc_.size(), {});
  for (size_t i = 0; i < scc_.size(); i++) {
    if (component_to_ideal_[i].empty()) {
      FindRightIdeals(i);
    }
  }

  std::vector<std::vector<ElementIndex>> result;
  result.reserve(component_to_ideal_.size());
  for (size_t i = 0; i < component_to_ideal_.size(); i++) {
    result.push_back(std::vector<ElementIndex>{component_to_ideal_[i].begin(), component_to_ideal_[i].end()});
  }
  return result;
}

void RightIdealsFinder::FindRightIdeals(size_t current_component_index) {
  const auto& current_component = scc_[current_component_index];
  std::set<ElementIndex> right_ideal{current_component.begin(), current_component.end()};
  for (auto start : current_component) {
    for (auto end : monoid_[start].transitions) {
      auto start_component = element_to_component_[start];
      auto end_component = element_to_component_[end];
      if (start_component == end_component) {
        continue;
      }
      if (component_to_ideal_[end_component].empty()) {
        FindRightIdeals(end_component);
      }
      const auto& child = component_to_ideal_[end_component];
      std::copy(child.begin(), child.end(), std::inserter(right_ideal, right_ideal.begin()));
    }
  }
  component_to_ideal_[current_component_index] = std::move(right_ideal);
}

Topsorter::Topsorter(const TransformationMonoid& monoid) : monoid_(monoid) {}

std::vector<ElementIndex> Topsorter::Topsort() {
    used_.assign(monoid_.size(), false);
    topsort_.clear();
    for (size_t i = 0; i < monoid_.size(); i++) {
      if (used_[i]) {
        continue;
      }
      Topsort(i);
    }
    return topsort_;
}

void Topsorter::Topsort(ElementIndex index) {
    used_[index] = true;
    const auto& el = monoid_[index];

    for (auto next_index : el.transitions) {
        if (used_[next_index]) {
            continue;
        }
        Topsort(next_index);
    }

    topsort_.push_back(index);
}

SCCFinder::SCCFinder(const TransformationMonoid& monoid) : monoid_(monoid) {}

StronglyConnectedComponents SCCFinder::Find() {
    auto topsort = Topsorter(monoid_).Topsort();

    auto transposed = GetTransposedGraph(monoid_);

    return FindSCCs(transposed, topsort);
}

std::vector<ElementIndex> SCCFinder::GetComponent(const TransposedGraph& transposed_monoid, ElementIndex index) {
  std::vector<ElementIndex> component;
  GetComponent(transposed_monoid, index, component);
  return component;
}

void SCCFinder::GetComponent(const TransposedGraph& transposed_monoid, ElementIndex index, std::vector<ElementIndex>& component) {
  used_[index] = true;
  component.push_back(index);
  const auto& el = transposed_monoid[index];

  for (const auto& next_letter : el) {
    for (auto next_index : next_letter) {
      if (used_[next_index]) {
        continue;
      }
      GetComponent(transposed_monoid, next_index, component);
    }
  }
}

StronglyConnectedComponents SCCFinder::FindSCCs(const TransposedGraph& transposed_monoid, const std::vector<ElementIndex>& topsort) {
  StronglyConnectedComponents result;
  used_.assign(transposed_monoid.size(), false);
  for (auto index : topsort | std::ranges::views::reverse) {
    if (used_[index]) {
      continue;
    }
    result.push_back(GetComponent(transposed_monoid, index));
  }
  return result;
}

std::vector<std::vector<std::string>> IndicesToWords(const TransformationMonoid& monoid, const std::vector<std::vector<ElementIndex>>& indices) {
  return indices | std::ranges::views::transform([&monoid](const auto& vec) {
               return vec | std::ranges::views::transform([&monoid](const auto& index) {
                        return monoid[index].word;
                      })
                      | std::ranges::to<std::vector>();
             })
             | std::ranges::to<std::vector>();
}

}  // namespace stewkk::ptp
