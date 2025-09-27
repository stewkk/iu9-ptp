#include <gmock/gmock.h>

#include <fstream>
#include <ranges>

#include <stewkk/ptp/logic/monoid.hpp>

using ::testing::Eq;

namespace stewkk::ptp {

const static std::string kProjectDir = std::getenv("PWD");
const static std::string kSimpleOutputPath = std::format("{}/test/simple.dot", kProjectDir);

void VisualizeDot(std::ostream& out, const TransformationMonoid& monoid) {
  out << "digraph G {\n";
  for (const auto& element : monoid) {
    out << std::format("\"{}\"\n", element.word);
  }
  for (const auto& start : monoid) {
    for (auto [letter_index, transition_index] : std::ranges::views::enumerate(start.transitions)) {
      const auto& end = monoid[transition_index];
      const auto& letter = monoid[letter_index];
      out << std::format("\"{}\" -> \"{}\" [label=\"{}\"]\n", start.word, end.word, letter.word);
    }
  }
  out << "}\n";
}

TEST(MonoidVisualizationTest, Simple) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto monoid = MonoidBuilder(letter_transformations).Build();
  std::ifstream f{kSimpleOutputPath};
  std::string expected((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>());
  std::ostringstream dot_output;

  VisualizeDot(dot_output, monoid);

  std::ofstream tmp{"/tmp/out.dot"};
  tmp << dot_output.str();

  ASSERT_THAT(dot_output.str(), Eq(expected));
}

}  // namespace stewkk::ptp
