#include <stewkk/ptp/logic/strong_connectivity.hpp>

#include <queue>
#include <algorithm>
#include <ranges>
#include <set>
#include <unordered_set>

namespace stewkk::ptp {

namespace {

TransposedGraph GetTransposedGraph(const CayleyGraph& monoid) {
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

IdealsBuilder::IdealsBuilder(CayleyGraph monoid,
                                     CondensationGraph graph)
    : monoid_(std::move(monoid)), graph_(std::move(graph)), scc_(ToSCCs(graph_)) {}

std::vector<std::vector<ElementIndex>> IdealsBuilder::Build() {
  std::vector<std::vector<ElementIndex>> ideals;
  std::vector<size_t> prefix;
  Generate(prefix, ideals);
  return ideals;
}

void IdealsBuilder::Generate(std::vector<size_t>& prefix, std::vector<std::vector<ElementIndex>>& ideals) {
  auto ideal_components = GetDescentants(prefix);
  if (!ideal_components.empty()) {
    auto ideal_vertices
        = ideal_components
          | std::ranges::views::transform([this](const auto& component) { return scc_[component]; })
          | std::ranges::views::join | std::ranges::to<std::vector>();
    std::ranges::sort(ideal_vertices);
    ideals.push_back(std::move(ideal_vertices));
  }

  if (prefix.size() == graph_.transitions.size()) {
    return;
  }
  size_t from = 0;
  if (prefix.size() > 0) {
    from = prefix.back()+1;
  }
  prefix.push_back(0);
  for (size_t i = from; i < graph_.transitions.size(); i++) {
    prefix.back() = i;
    Generate(prefix, ideals);
  }
  prefix.pop_back();
}

std::vector<size_t> IdealsBuilder::GetDescentants(std::vector<size_t> vertices) {
  std::queue<size_t> q(vertices.begin(), vertices.end());
  enum class Color {
    kSource,
    kUsed,
    kNotUsed,
  };
  std::vector<Color> used(graph_.transitions.size(), Color::kNotUsed);
  for (auto source : vertices) {
    used[source] = Color::kSource;
  }
  std::vector<size_t> result;
  result.reserve(graph_.transitions.size());
  while (!q.empty()) {
    auto cur = q.front();
    q.pop();
    if (used[cur] == Color::kUsed) {
      continue;
    }
    if (used[cur] != Color::kSource) {
      used[cur] = Color::kUsed;
    }
    result.push_back(cur);
    for (auto next : graph_.transitions[cur]) {
      if (used[next] == Color::kUsed) {
        continue;
      }
      if (used[next] == Color::kSource) {
        return {};
      }
      q.push(next);
    }
  }
  return result;
}

Topsorter::Topsorter(const CayleyGraph& monoid) : monoid_(monoid) {}

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

CondensationGraphBuilder::CondensationGraphBuilder(const CayleyGraph& monoid) : monoid_(monoid) {}

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
        continue;
      }
      GetComponent(transposed_monoid, next_index, component);
    }
  }
}

CondensationGraph CondensationGraphBuilder::Build(const TransposedGraph& transposed_monoid, const std::vector<ElementIndex>& topsort) {
  element_to_component_.assign(transposed_monoid.size(), -1);
  size_t component = 0;
  for (auto index : topsort | std::ranges::views::reverse) {
    if (element_to_component_[index] != -1) {
      continue;
    }
    GetComponent(transposed_monoid, index, component);
    component++;
  }

  std::vector<std::unordered_set<size_t>> transitions(component);
  for (size_t element = 0; element < monoid_.size(); element++) {
    auto elements_component = element_to_component_[element];
    for (auto transition : monoid_[element].transitions) {
      auto transitions_component = element_to_component_[transition];
      if (transitions_component == elements_component) {
        continue;
      }
      transitions[elements_component].insert(transitions_component);
    }
  }

  return ToCondensationGraph(std::move(element_to_component_), std::move(transitions));
}

std::vector<std::vector<std::string>> IndicesToWords(const CayleyGraph& monoid, const std::vector<std::vector<ElementIndex>>& indices) {
  return indices | std::ranges::views::transform([&monoid](const auto& vec) {
               return IndicesToWords(monoid, vec);
             })
             | std::ranges::to<std::vector>();
}

std::vector<std::string> IndicesToWords(const CayleyGraph& monoid, const std::vector<ElementIndex>& indices) {
  return indices | std::ranges::views::transform([&monoid](const auto& index) {
           return monoid[index].word;
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

std::vector<std::vector<std::string>> BuildIdeals(
    const std::vector<std::vector<std::string>>& left,
    const std::vector<std::vector<std::string>>& right) {
  std::vector<std::vector<std::string>> result;
  auto set = left | std::ranges::views::transform([](const auto& ideal) {
    auto tmp = ideal;
    std::ranges::sort(tmp);
    return tmp;
  }) | std::ranges::to<std::set>();

  std::copy_if(right.begin(), right.end(), std::back_inserter(result), [&set](const auto& el) {
    auto tmp = el;
    std::ranges::sort(tmp);
    return set.contains(tmp);
  });
  return result;
}

}  // namespace stewkk::ptp
