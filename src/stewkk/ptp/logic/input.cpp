#include <stewkk/ptp/logic/input.hpp>

#include <unordered_set>

namespace stewkk::ptp {

namespace {

std::vector<WordDTO> ReadWordsSorted(std::istream& input) {
  size_t words_count;
  input >> words_count;

  std::unordered_set<WordDTO> words_set;
  words_set.reserve(words_count);
  for (size_t i = 0; i < words_count; i++) {
    WordDTO word;
    input >> word;
    words_set.insert(std::move(word));
  }

  std::vector<WordDTO> words(words_set.begin(), words_set.end());
  std::sort(words.begin(), words.end());
  return words;
}

WordToTransformationDTO ReadTransformations(std::istream& input) {
  size_t actions_count;
  input >> actions_count;

  WordToTransformationDTO mapping;
  for (size_t i = 0; i < actions_count; i++) {
    WordDTO lhs;
    WordDTO rhs;
    WordDTO letter;
    input >> lhs >> letter >> rhs;
    mapping[letter][lhs] = rhs;
  }

  return mapping;
}

}  // namespace

InputDTO ProcessInput(std::istream& input) {
  return {
      .words = ReadWordsSorted(input),
      .transformations = ReadTransformations(input),
  };
}

}  // namespace stewkk::ptp
