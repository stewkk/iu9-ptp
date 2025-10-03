#pragma once

#include <vector>
#include <set>

#include <stewkk/ptp/models/monoid.hpp>
#include <stewkk/ptp/models/strong_connectivity.hpp>
#include <stewkk/ptp/logic/monoid.hpp>

namespace stewkk::ptp {

class RightIdealsFinder {
public:
    RightIdealsFinder(const TransformationMonoid& monoid, const StronglyConnectedComponents& scc);

    std::vector<std::vector<ElementIndex>> FindRightIdeals();
    void FindRightIdeals(size_t current_component_index);

private:
    const TransformationMonoid& monoid_;
    const StronglyConnectedComponents& scc_;
    std::vector<std::set<ElementIndex>> component_to_ideal_;
    std::vector<char> element_to_component_;
};

class Topsorter {
public:
    explicit Topsorter(const TransformationMonoid& monoid);
    std::vector<ElementIndex> Topsort();
    void Topsort(ElementIndex index);

private:
    std::vector<char> used_;
    std::vector<ElementIndex> topsort_;
    const TransformationMonoid& monoid_;
};

using TransposedGraph = std::vector<std::vector<std::vector<ElementIndex>>>;

class SCCFinder {
public:
    explicit SCCFinder(const TransformationMonoid& monoid);
    StronglyConnectedComponents Find();

private:
    StronglyConnectedComponents FindSCCs(const TransposedGraph& transposed_monoid, const std::vector<ElementIndex>& topsort);
    std::vector<ElementIndex> GetComponent(const TransposedGraph& transposed_monoid,
                                           ElementIndex index);
    void GetComponent(const TransposedGraph& transposed_monoid, ElementIndex index,
                      std::vector<ElementIndex>& component);

  private:
    std::vector<char> used_;
    const TransformationMonoid& monoid_;
};

std::vector<std::vector<std::string>> IndicesToWords(const TransformationMonoid& monoid, const std::vector<std::vector<ElementIndex>>& indices);

}  // namespace stewkk::ptp
