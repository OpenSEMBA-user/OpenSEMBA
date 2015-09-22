// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
