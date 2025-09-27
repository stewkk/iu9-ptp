#include <gmock/gmock.h>

#include <fstream>
#include <format>

#include <stewkk/ptp/models/dto.hpp>
#include <stewkk/ptp/logic/input.hpp>

using ::testing::Eq;

namespace stewkk::ptp {

const static std::string kProjectDir = std::getenv("PWD");
const static std::string kBasicDataPath = std::format("{}/test/input_basic.txt", kProjectDir);

TEST(ProcessInputTest, ReturnsSortedWordSet) {
  std::ifstream input{kBasicDataPath};

  auto got = ProcessInput(input);

  ASSERT_THAT(got.words, Eq(std::vector<WordDTO>{
        "x",
        "y",
        "z",
      }));
}

TEST(ProcessInputTest, ReturnsWordToTransformationMapping) {
  std::ifstream input{kBasicDataPath};

  auto got = ProcessInput(input);

  ASSERT_THAT(got.transformations, Eq(WordToTransformationDTO{
                               {"a",
                                {
                                    {"x", "x"},
                                    {"y", "y"},
                                    {"z", "x"},
                                }},
                               {"b",
                                {
                                    {"x", "z"},
                                    {"y", "y"},
                                    {"z", "x"},
                                }},
                               {"c",
                                {
                                    {"x", "z"},
                                    {"y", "z"},
                                    {"z", "z"},
                                }},
                           }));
}

}  // namespace stewkk::ptp
