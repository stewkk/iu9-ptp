#include <gmock/gmock.h>

#include <fstream>

#include <stewkk/ptp/logic/dot.hpp>
#include <stewkk/ptp/logic/strong_connectivity.hpp>

using ::testing::Eq;

namespace stewkk::ptp {

TEST(StrongConnectivityTest, FindsSCCs) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto monoid = MonoidBuilder(letter_transformations).Build();

  auto got = SCCFinder(monoid).Find();

  ASSERT_THAT(got, Eq(StronglyConnectedComponents{{ 1, 4 }, { 0, 3 }, { 2, 5 }}));
}

TEST(StrongConnectivityTest, FindRightIdeals) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto monoid = MonoidBuilder(letter_transformations).Build();
  auto scc = SCCFinder(monoid).Find();

  auto right_ideals = RightIdealsFinder(monoid, scc).FindRightIdeals();

  ASSERT_THAT(right_ideals,
              Eq(std::vector<std::vector<ElementIndex>>{{ 0, 1, 2, 3, 4, 5 }, { 0, 2, 3, 5 }, { 2, 5 }}));
}

TEST(StrongConnectivityTest, FindRightIdealsComplex) {
  LetterToTransformation letter_transformations{
      {'a', {2, 1, 2}},
      {'c', {2, 0, 1}},
  };
  auto monoid = MonoidBuilder(letter_transformations).Build();
  auto scc = SCCFinder(monoid).Find();
  std::ofstream tmp{"/tmp/out3.dot"};
  VisualizeDot(tmp, monoid);

  auto right_ideals = RightIdealsFinder(monoid, scc).FindRightIdeals();

  ASSERT_THAT(scc, Eq(StronglyConnectedComponents{{1, 9, 4},
                                                  {8, 22, 18, 14, 23, 19},
                                                  {3, 17, 12, 7, 21, 13},
                                                  {0, 10, 5, 2, 15, 6},
                                                  {11, 20, 16}}));
  ASSERT_THAT(
      right_ideals,
      Eq(std::vector<std::vector<ElementIndex>>{
          {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
          {8, 11, 14, 16, 18, 19, 20, 22, 23},
          {3, 7, 11, 12, 13, 16, 17, 20, 21},
          {0, 2, 5, 6, 10, 11, 15, 16, 20},
          {11, 16, 20}}));
}

}  // namespace stewkk::ptp
