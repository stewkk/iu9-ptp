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
  ASSERT_THAT(got[3].transformation, Eq(Transformation{2, 1, 2}));
  ASSERT_THAT(got[4].transformation, Eq(Transformation{0, 1, 2}));
  ASSERT_THAT(got[5].transformation, Eq(Transformation{0, 0, 0}));
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
  ASSERT_THAT(std::ranges::contains(got, "c", &MonoidElement::word), IsFalse());
}

TEST(MonoidBuilderTest, BuildsTransitions) {
  LetterToTransformation letter_transformations{
      {'a', {0, 1, 0}},
      {'b', {2, 1, 0}},
      {'c', {2, 2, 2}},
  };
  MonoidBuilder builder(letter_transformations);

  auto got = builder.Build();

  ASSERT_THAT(got[0].transitions, Eq(std::vector<size_t>{0, 3, 2}));
}

}  // namespace stewkk::ptp
