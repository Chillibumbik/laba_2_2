#pragma once

#include <stdexcept>
#include <string>

namespace Errors {
    inline std::logic_error Immutable() {
        return std::logic_error("Immutable object");
    }

    inline std::out_of_range IndexOutOfRange() {
        return std::out_of_range("Index out of range");
    }

    inline std::invalid_argument InvalidArgument(const std::string& message = "") {
        return std::invalid_argument("Invalid argument" + (message.empty() ? "" : ": " + message));
    }

    inline std::out_of_range EmptyArray() {
        return std::out_of_range("Empty array");
    }

    inline std::out_of_range EmptyList() {
        return std::out_of_range("Empty list");
    }

    inline std::invalid_argument IncompatibleTypes() {
        return std::invalid_argument("Incompatible types");
    }

    inline std::runtime_error EmptyValue() {
        return std::runtime_error("Empty value");
    }

    inline std::invalid_argument NegativeSize() {
        return std::invalid_argument("Negative size not allowed");
    }

    inline std::out_of_range InvalidIndices() {
        return std::out_of_range("Invalid indices");
    }

    inline std::invalid_argument NegativeCount() {
        return std::invalid_argument("Negative count");
    }

    inline std::invalid_argument NullList() {
        return std::invalid_argument("Null list");
    }


}
