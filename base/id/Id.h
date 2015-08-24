/*
 * Id.h
 *
 *  Created on: 22/6/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_ID_ID_H_
#define COMMON_BASE_ID_ID_H_

#include <iostream>
#include <sstream>
#include <string>

#include "Types.h"

template <typename T>
class Id {
    template <typename I>
    friend std::istream& operator>>(std::istream&  input,       Id<I>& id);
    template <typename I>
    friend std::ostream& operator<<(std::ostream& output, const Id<I>& id);
public:
    Id();
    explicit Id(const UInt id);
    Id(const Id& rhs);
    virtual ~Id();

    Id& operator =(const Id& rhs);
    Id& operator+=(const Id& rhs);

    bool operator==(const Id& rhs) const;
    bool operator!=(const Id& rhs) const;
    bool operator< (const Id& rhs) const;
    bool operator<=(const Id& rhs) const;
    bool operator> (const Id& rhs) const;
    bool operator>=(const Id& rhs) const;

    Id  operator+ (const Id& rhs) const;
    Id& operator++();
    Id  operator++(int);

    UInt        toUInt() const;
    std::string toStr() const;

private:
    UInt id_;
};

#include "Id.hpp"

#endif /* COMMON_BASE_ID_ID_H_ */
