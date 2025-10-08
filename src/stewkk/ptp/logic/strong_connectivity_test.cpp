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
  auto monoid = CayleyGraphBuilder(letter_transformations).Build();

  auto condensation_graph = CondensationGraphBuilder(monoid).Build();
  auto got = ToSCCs(condensation_graph);

  ASSERT_THAT(IndicesToWords(monoid, got),
              Eq(std::vector<std::vector<std::string>>{{"b", "bb"}, {"a", "ab"}, {"c", "ca"}}));
}

TEST(StrongConnectivityTest, FindRightIdeals) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto monoid = CayleyGraphBuilder(letter_transformations).Build();
  auto condensation_graph = CondensationGraphBuilder(monoid).Build();

  auto right_ideals = IdealsBuilder(monoid, condensation_graph).Build();

  ASSERT_THAT(IndicesToWords(monoid, right_ideals),
              Eq(std::vector<std::vector<std::string>>{
                  {"a", "b", "c", "ab", "bb", "ca"}, {"a", "c", "ab", "ca"}, {"c", "ca"}}));
}

TEST(StrongConnectivityTest, FindRightIdealsComplex) {
  LetterToTransformation letter_transformations{
      {'a', {2, 1, 2}},
      {'c', {2, 0, 1}},
  };
  auto monoid = CayleyGraphBuilder(letter_transformations).Build();
  auto condensation_graph = CondensationGraphBuilder(monoid).Build();
  std::ofstream tmp{"/tmp/out3.dot"};
  VisualizeDot(tmp, monoid);

  auto scc = ToSCCs(condensation_graph);
  auto right_ideals = IdealsBuilder(monoid, condensation_graph).Build();

  ASSERT_THAT(IndicesToWords(monoid, scc),
              Eq(std::vector<std::vector<std::string>>{
                  {"c", "cc", "ccc"},
                  {"cca", "ccac", "ccaca", "ccacc", "ccacac", "ccacacc"},
                  {"ca", "cac", "caca", "cacc", "cacac", "cacacc"},
                  {"a", "ac", "aca", "acc", "acac", "acacc"},
                  {"acca", "accac", "accacc"}}));
  ASSERT_THAT(IndicesToWords(monoid, right_ideals),
              Eq(std::vector<std::vector<std::string>>{
                  {"a",     "c",     "ac",    "ca",    "cc",     "aca",    "acc",    "cac",
                   "cca",   "ccc",   "acac",  "acca",  "caca",   "cacc",   "ccac",   "acacc",
                   "accac", "cacac", "ccaca", "ccacc", "accacc", "cacacc", "ccacac", "ccacacc"},
                  {"cca", "acca", "ccac", "accac", "ccaca", "ccacc", "accacc", "ccacac", "ccacacc"},
                  {"ca", "cac", "cca", "acca", "caca", "cacc", "ccac", "accac", "cacac", "ccaca",
                   "ccacc", "accacc", "cacacc", "ccacac", "ccacacc"},
                  {"a",     "ac",    "ca",    "aca",    "acc",    "cac",    "cca",
                   "acac",  "acca",  "caca",  "cacc",   "ccac",   "acacc",  "accac",
                   "cacac", "ccaca", "ccacc", "accacc", "cacacc", "ccacac", "ccacacc"},
                  {"a", "ac", "aca", "acc", "cca", "acac", "acca", "ccac", "acacc", "accac",
                   "ccaca", "ccacc", "accacc", "ccacac", "ccacacc"},
                  {"ca", "cac", "acca", "caca", "cacc", "accac", "cacac", "accacc", "cacacc"},
                  {"a", "ac", "ca", "aca", "acc", "cac", "acac", "acca", "caca", "cacc", "acacc",
                   "accac", "cacac", "accacc", "cacacc"},
                  {"a", "ac", "aca", "acc", "acac", "acca", "acacc", "accac", "accacc"},
                  {"acca", "accac", "accacc"}}));
}

TEST(StrongConnectivityTest, FindLeftIdeals) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto monoid = CayleyGraphBuilder(letter_transformations, LeftComposition, LeftWordComposition).Build();
  auto condensation_graph = CondensationGraphBuilder(monoid).Build();

  auto left_ideals = IdealsBuilder(monoid, condensation_graph).Build();

  ASSERT_THAT(IndicesToWords(monoid, left_ideals),
              Eq(std::vector<std::vector<std::string>>{{"a", "b", "c", "ca", "ab", "bb"},
                                                       {"c", "ab"},
                                                       {"a", "c", "ca", "ab"},
                                                       {"c", "ca", "ab"},
                                                       {"c"},
                                                       {"a", "c", "ca"},
                                                       {"c", "ca"},
                                                       {"a", "ca"},
                                                       {"ca"}}));
}

TEST(StrongConnectivityTest, FindTwoSidedIdeals) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto left_graph = CayleyGraphBuilder(letter_transformations, LeftComposition, LeftWordComposition).Build();
  auto right_graph = CayleyGraphBuilder(letter_transformations).Build();

  auto left_condensation_graph = CondensationGraphBuilder(left_graph).Build();
  auto right_condensation_graph = CondensationGraphBuilder(right_graph).Build();

  auto left_ideals = IdealsBuilder(left_graph, left_condensation_graph).Build();
  auto right_ideals = IdealsBuilder(right_graph, right_condensation_graph).Build();

  auto ideals = BuildIdeals(IndicesToWords(left_graph, left_ideals),
                            IndicesToWords(right_graph, right_ideals));

  ASSERT_THAT(ideals, Eq(std::vector<std::vector<std::string>>{
                          {"a", "b", "c", "ab", "bb", "ca"}, {"a", "c", "ab", "ca"}, {"c", "ca"}}));
}

}  // namespace stewkk::ptp
