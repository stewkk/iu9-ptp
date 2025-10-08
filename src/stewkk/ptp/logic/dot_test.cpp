#include <gmock/gmock.h>

#include <fstream>

#include <stewkk/ptp/logic/monoid.hpp>
#include <stewkk/ptp/logic/dot.hpp>

using ::testing::Eq;

namespace stewkk::ptp {

const static std::string kProjectDir = std::getenv("PWD");
const static std::string kSimpleOutputPath = std::format("{}/test/simple.dot", kProjectDir);

TEST(MonoidVisualizationTest, Simple) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  auto monoid = CayleyGraphBuilder(letter_transformations).Build();
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
