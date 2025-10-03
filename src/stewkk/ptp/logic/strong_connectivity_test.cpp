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

  ASSERT_THAT(IndicesToWords(monoid, got),
              Eq(std::vector<std::vector<std::string>>{{"b", "bb"}, {"a", "ab"}, {"c", "ca"}}));
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

  ASSERT_THAT(IndicesToWords(monoid, right_ideals),
              Eq(std::vector<std::vector<std::string>>{
                  {"a", "b", "c", "ab", "bb", "ca"}, {"a", "c", "ab", "ca"}, {"c", "ca"}}));
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

  ASSERT_THAT(IndicesToWords(monoid, scc),
              Eq(std::vector<std::vector<std::string>>{
                  {"c", "ccc", "cc"},
                  {"cca", "ccacac", "ccaca", "ccac", "ccacacc", "ccacc"},
                  {"ca", "cacac", "caca", "cac", "cacacc", "cacc"},
                  {"a", "acac", "aca", "ac", "acacc", "acc"},
                  {"acca", "accacc", "accac"}}));
  ASSERT_THAT(IndicesToWords(monoid, right_ideals),
              Eq(std::vector<std::vector<std::string>>{
                  {"a",     "c",     "ac",    "ca",    "cc",     "aca",    "acc",    "cac",
                   "cca",   "ccc",   "acac",  "acca",  "caca",   "cacc",   "ccac",   "acacc",
                   "accac", "cacac", "ccaca", "ccacc", "accacc", "cacacc", "ccacac", "ccacacc"},
                  {"cca", "acca", "ccac", "accac", "ccaca", "ccacc", "accacc", "ccacac", "ccacacc"},
                  {"ca", "cac", "acca", "caca", "cacc", "accac", "cacac", "accacc", "cacacc"},
                  {"a", "ac", "aca", "acc", "acac", "acca", "acacc", "accac", "accacc"},
                  {"acca", "accac", "accacc"}}));
}

TEST(StrongConnectivityTest, FindLeftIdeals) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto monoid = MonoidBuilder(letter_transformations, LeftComposition, LeftWordComposition).Build();
  auto scc = SCCFinder(monoid).Find();

  auto left_ideals = RightIdealsFinder(monoid, scc).FindRightIdeals();

  ASSERT_THAT(IndicesToWords(monoid, left_ideals),
              Eq(std::vector<std::vector<std::string>>{}));
}

}  // namespace stewkk::ptp
