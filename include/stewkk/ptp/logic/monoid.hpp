#pragma once

#include <set>
#include <vector>

#include <stewkk/ptp/models/transformation.hpp>

namespace stewkk::ptp {

class MonoidBuilder {
public:
    explicit MonoidBuilder(const WordToTransformation& letter_transformations);
    WordToTransformation Build();

private:
    void AddComposition(const std::pair<Word, Transformation>& lhs, const std::pair<Word, Transformation>& rhs);

private:
    const WordToTransformation& letter_transformations_;
    std::set<Transformation> monoid_transformations_;
    std::vector<std::pair<Word, Transformation>> monoid_elements_;
};

}  // stewkk::ptp
