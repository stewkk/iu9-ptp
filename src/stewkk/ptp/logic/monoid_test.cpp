#include <gmock/gmock.h>

#include <stewkk/ptp/logic/monoid.hpp>

using ::testing::Eq;
using ::testing::IsFalse;

namespace stewkk::ptp {

TEST(MonoidBuilderTest, FindsClosureByTransformationComposition) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  MonoidBuilder builder(std::move(letter_transformations));

  auto got = builder.Build();

  ASSERT_THAT(got.size(), Eq(6));
  ASSERT_THAT(got.at("ab").transformation, Eq(Transformation{2, 1, 2}));
  ASSERT_THAT(got.at("bb").transformation, Eq(Transformation{0, 1, 2}));
  ASSERT_THAT(got.at("ca").transformation, Eq(Transformation{0, 0, 0}));
}

TEST(MonoidBuilderTest, EliminatesDuplicateTransformations) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 1, 0}},
  };
  MonoidBuilder builder(letter_transformations);

  auto got = builder.Build();

  ASSERT_THAT(got.size(), Eq(4));
  ASSERT_THAT(got.contains("c"), IsFalse());
}

TEST(MonoidBuilderTest, BuildsTransitions) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  MonoidBuilder builder(letter_transformations);

  auto got = builder.Build();

  ASSERT_THAT(got.at("a").transitions, Eq(std::vector<size_t>{0, 3, 2}));
}

}  // namespace stewkk::ptp
