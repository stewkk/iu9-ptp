#include <stewkk/ptp/logic/strong_connectivity.hpp>

#include <algorithm>
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

CondensationGraph ToCondensationGraph(std::vector<int32_t> element_to_component,
                                      std::vector<std::unordered_set<size_t>> transitions) {
  return CondensationGraph{
      std::vector<size_t>{element_to_component.begin(), element_to_component.end()},
      transitions | std::ranges::views::transform([](const auto& set) {
        auto vec = set | std::ranges::to<std::vector>();
        std::ranges::sort(vec);
        return vec;
      }) | std::ranges::to<std::vector>(),
  };
}

}  // namespace

RightIdealsFinder::RightIdealsFinder(const TransformationMonoid& monoid,
                                     CondensationGraph graph)
    : monoid_(monoid), element_to_component_(graph.element_to_component), scc_(ToSCCs(std::move(graph))) {}

std::vector<std::vector<ElementIndex>> RightIdealsFinder::FindRightIdeals() {
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

CondensationGraphBuilder::CondensationGraphBuilder(const TransformationMonoid& monoid) : monoid_(monoid) {}

CondensationGraph CondensationGraphBuilder::Build() {
    auto topsort = Topsorter(monoid_).Topsort();

    auto transposed = GetTransposedGraph(monoid_);

    return Build(transposed, topsort);
}

void CondensationGraphBuilder::GetComponent(const TransposedGraph& transposed_monoid, ElementIndex index, size_t component) {
  element_to_component_[index] = component;
  const auto& el = transposed_monoid[index];

  for (const auto& next_letter : el) {
    for (auto next_index : next_letter) {
      if (element_to_component_[next_index] != -1) {
        transitions_[component].insert(element_to_component_[next_index]);
        continue;
      }
      GetComponent(transposed_monoid, next_index, component);
    }
  }
}

CondensationGraph CondensationGraphBuilder::Build(const TransposedGraph& transposed_monoid, const std::vector<ElementIndex>& topsort) {
  element_to_component_.assign(transposed_monoid.size(), -1);
  transitions_.clear();
  size_t component = 0;
  for (auto index : topsort | std::ranges::views::reverse) {
    if (element_to_component_[index] != -1) {
      continue;
    }
    transitions_.push_back({});
    GetComponent(transposed_monoid, index, component);
    component++;
  }
  return ToCondensationGraph(std::move(element_to_component_), std::move(transitions_));
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

StronglyConnectedComponents ToSCCs(const CondensationGraph& graph) {
  auto components_count = std::ranges::max(graph.element_to_component)+1;
  StronglyConnectedComponents result(components_count);
  for (auto [el, component] : graph.element_to_component | std::ranges::views::enumerate) {
    result[component].push_back(el);
  }
  return result;
}

}  // namespace stewkk::ptp
