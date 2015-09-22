/*
 * Array.h
 *
 *  Created on: 24/6/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_SEQUENCE_ARRAY_H_
#define COMMON_BASE_SEQUENCE_ARRAY_H_

#include <array>
#include <type_traits>

#include "Sequence.h"

template<typename T, Int D>
class Array : public Sequence<T> {
public:
    Array();
    Array(const T&);
    Array(const std::array<T,D>&);
    Array(const Array&);
    virtual ~Array();

    Array& operator=(const Array& rhs);

    bool empty() const { return element_.empty(); }
    UInt size () const { return element_.size();  }

    typename Reference<T>::type      get(const UInt i);
    typename ConstReference<T>::type get(const UInt i) const;

private:
    std::array<T,D> element_;
};

#include "Array.hpp"

#endif /* COMMON_BASE_SEQUENCE_ARRAY_H_ */
