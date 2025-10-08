#pragma once

#include <vector>
#include <set>
#include <unordered_set>

#include <stewkk/ptp/models/monoid.hpp>
#include <stewkk/ptp/models/strong_connectivity.hpp>
#include <stewkk/ptp/logic/monoid.hpp>

namespace stewkk::ptp {

class RightIdealsFinder {
public:
    RightIdealsFinder(const TransformationMonoid& monoid, CondensationGraph graph);
    std::vector<std::vector<ElementIndex>> FindRightIdeals();

private:
    void FindRightIdeals(size_t current_component_index);

private:
    const TransformationMonoid& monoid_;
    std::vector<std::set<ElementIndex>> component_to_ideal_;
    std::vector<size_t> element_to_component_;
    const StronglyConnectedComponents scc_;
};

class Topsorter {
public:
    explicit Topsorter(const TransformationMonoid& monoid);
    std::vector<ElementIndex> Topsort();

private:
    void Topsort(ElementIndex index);

private:
    std::vector<char> used_;
    std::vector<ElementIndex> topsort_;
    const TransformationMonoid& monoid_;
};

using TransposedGraph = std::vector<std::vector<std::vector<ElementIndex>>>;

class CondensationGraphBuilder {
public:
    explicit CondensationGraphBuilder(const TransformationMonoid& monoid);
    CondensationGraph Build();

private:
    CondensationGraph Build(const TransposedGraph& transposed_monoid, const std::vector<ElementIndex>& topsort);
    void GetComponent(const TransposedGraph& transposed_monoid,
                                           ElementIndex index, size_t component);

  private:
    std::vector<int32_t> element_to_component_;
    std::vector<std::unordered_set<size_t>> transitions_;
    const TransformationMonoid& monoid_;
};

std::vector<std::vector<std::string>> IndicesToWords(const TransformationMonoid& monoid, const std::vector<std::vector<ElementIndex>>& indices);

StronglyConnectedComponents ToSCCs(const CondensationGraph& graph);

}  // namespace stewkk::ptp
