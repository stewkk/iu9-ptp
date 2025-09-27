#pragma once

#include <map>

#include <stewkk/ptp/models/monoid.hpp>

namespace stewkk::ptp {

class MonoidBuilder {
public:
    explicit MonoidBuilder(const LetterToTransformation& letter_transformations);
    TransformationMonoid Build();

private:
    void AddComposition(size_t element_index, size_t letter_index);

private:
    TransformationMonoid monoid_elements_;
    std::map<Transformation, size_t> monoid_transformations_;
};

}  // stewkk::ptp
