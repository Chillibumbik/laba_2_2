#pragma once
#include <any>
#include <iostream>
#include <string>
#include <typeinfo>
#include <stdexcept>

#include "errors.hpp"

class BoxedAny {
private:
    std::any value;

public:
    BoxedAny() = default;

    template<typename T>
    BoxedAny(const T& val) : value(val) {}

    template<typename T>
    T& As() {
        if (!value.has_value()) throw Errors::EmptyValue();
        if (value.type() != typeid(T)) throw std::bad_any_cast();
        return std::any_cast<T&>(value);
    }

    template<typename T>
    bool Is() const {
        return value.type() == typeid(T);
    }

    std::string TypeName() const {
        return value.has_value() ? value.type().name() : "empty";
    }

    friend std::ostream& operator<<(std::ostream& os, const BoxedAny& obj) {
        if (obj.value.type() == typeid(int)) os << std::any_cast<int>(obj.value);
        else if (obj.value.type() == typeid(double)) os << std::any_cast<double>(obj.value);
        else if (obj.value.type() == typeid(std::string)) os << std::any_cast<std::string>(obj.value);
        else os << "<unsupported type: " << obj.value.type().name() << ">";
        return os;
    }
};
