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
