#include <stewkk/ptp/logic/dot.hpp>

#include <format>
#include <ranges>

namespace stewkk::ptp {

void VisualizeDot(std::ostream& out, const CayleyGraph& monoid) {
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

}  // namespace stewkk::ptp
