

#pragma once

#include <cstddef>
#include <iostream>
#include <string>

namespace SEMBA {
namespace Class {

template <typename T>
class Identification {
    template <typename I>
    friend std::istream& operator>>(std::istream&  input,
                                    Identification<I>& id);
    template <typename I>
    friend std::ostream& operator<<(std::ostream& output,
                                    const Identification<I>& id);
public:
    Identification();
    explicit Identification(const std::size_t id);
    Identification(const Identification& rhs);
    virtual ~Identification();

    Identification& operator =(const Identification& rhs);
    Identification& operator+=(const Identification& rhs);

    bool operator==(const Identification& rhs) const;
    bool operator!=(const Identification& rhs) const;
    bool operator< (const Identification& rhs) const;
    bool operator<=(const Identification& rhs) const;
    bool operator> (const Identification& rhs) const;
    bool operator>=(const Identification& rhs) const;

    Identification  operator+ (const Identification& rhs) const;
    Identification& operator++();
    Identification  operator++(int);

    std::size_t toInt() const;
    std::string toStr() const;

private:
    std::size_t id_;
};

} /* namespace Class */
} /* namespace SEMBA */

#include "Identification.hpp"

