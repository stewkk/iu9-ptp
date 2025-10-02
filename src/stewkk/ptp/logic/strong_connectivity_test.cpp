#include <gmock/gmock.h>

#include <ranges>

#include <stewkk/ptp/models/monoid.hpp>
#include <stewkk/ptp/models/strong_connectivity.hpp>
#include <stewkk/ptp/logic/monoid.hpp>
#include <stewkk/ptp/logic/dot.hpp>

using ::testing::Eq;

namespace stewkk::ptp {

TransformationMonoid GetTransposedMonoid(const TransformationMonoid& monoid) {
  TransformationMonoid result(monoid.size());
  for (const auto& [start_index, start] : std::ranges::views::enumerate(monoid)) {
    auto& result_start = result[start_index];
    result_start.word = start.word;
    result_start.transformation = start.transformation;
    result_start.transitions.assign(start.transitions.size(), 0);
  }

  for (const auto& [start_index, start] : std::ranges::views::enumerate(monoid)) {
    for (auto [letter_index, end_index] : std::ranges::views::enumerate(start.transitions)) {
      result[end_index].transitions[letter_index] = start_index;
    }
  }

  return result;
}

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

Topsorter::Topsorter(const TransformationMonoid& monoid) : monoid_(monoid) {}

std::vector<ElementIndex> Topsorter::Topsort() {
    used_.assign(monoid_.size(), false);
    topsort_.clear();
    Topsort(0);
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

class SCCFinder {
public:
    explicit SCCFinder(const TransformationMonoid& monoid);
    StronglyConnectedComponents Find();

private:
    StronglyConnectedComponents FindSCCs(const TransformationMonoid& reversed_monoid, const std::vector<ElementIndex>& topsort);
    std::vector<ElementIndex> GetComponent(const TransformationMonoid& transposed_monoid,
                                           ElementIndex index);
    void GetComponent(const TransformationMonoid& transposed_monoid, ElementIndex index,
                      std::vector<ElementIndex>& component);

  private:
    std::vector<char> used_;
    const TransformationMonoid& monoid_;
};

SCCFinder::SCCFinder(const TransformationMonoid& monoid) : monoid_(monoid) {}

StronglyConnectedComponents SCCFinder::Find() {
    auto topsort = Topsorter(monoid_).Topsort();

    auto transposed = GetTransposedMonoid(monoid_);

    return FindSCCs(transposed, topsort);
}

std::vector<ElementIndex> SCCFinder::GetComponent(const TransformationMonoid& transposed_monoid, ElementIndex index) {
  std::vector<ElementIndex> component;
  GetComponent(transposed_monoid, index, component);
  return component;
}

void SCCFinder::GetComponent(const TransformationMonoid& transposed_monoid, ElementIndex index, std::vector<ElementIndex>& component) {
  used_[index] = true;
  component.push_back(index);
  const auto& el = transposed_monoid[index];

  for (auto next_index : el.transitions) {
    if (used_[next_index]) {
      continue;
    }
    GetComponent(transposed_monoid, next_index, component);
  }
}

StronglyConnectedComponents SCCFinder::FindSCCs(const TransformationMonoid& transposed_monoid, const std::vector<ElementIndex>& topsort) {
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


TEST(StrongConnectivityTest, FindsSCCs) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto monoid = MonoidBuilder(letter_transformations).Build();

  auto got = SCCFinder(monoid).Find();

  // TODO: compare as multisets
  ASSERT_THAT(got, Eq(StronglyConnectedComponents{{ 0, 4, 1, 3 }, { 2, 5 }}));
}

}  // namespace stewkk::ptp
