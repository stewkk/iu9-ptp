#include <gmock/gmock.h>

#include <stewkk/ptp/logic/monoid.hpp>

using ::testing::Eq;

namespace stewkk::ptp {

TEST(GetTransformationMonoidTest, FindsClosureByTransformationComposition) {
  WordToTransformation letter_transformations{
      {"a", {0, 1, 0}},
      {"b", {2, 1, 0}},
      {"c", {2, 2, 2}},
  };
  MonoidBuilder builder(letter_transformations);

  WordToTransformation got = builder.Build();

  ASSERT_THAT(got.at("ab"), Eq(Transformation{2, 1, 2}));
  ASSERT_THAT(got.at("bb"), Eq(Transformation{0, 1, 2}));
  ASSERT_THAT(got.at("cb"), Eq(Transformation{0, 0, 0}));
  ASSERT_THAT(got.size(), Eq(6));
}

}  // namespace stewkk::ptp
