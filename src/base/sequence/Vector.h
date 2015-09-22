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
 * Vector.h
 *
 *  Created on: 23/6/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_SEQUENCE_VECTOR_H_
#define COMMON_BASE_SEQUENCE_VECTOR_H_

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "Sequence.h"
#include "VectorPtr.h"

template<typename T>
class Vector : public Sequence<T> {
public:
    Vector();
    Vector(const T&);
    Vector(const std::vector<T>&);
    Vector(const Vector<T>&);
    virtual ~Vector();

    Vector<T>& operator=(const Vector<T>& rhs);

    void resize (const UInt nS) { element_.resize (nS);    }
    void reserve(const UInt nS) { element_.reserve(nS);    }
    bool empty() const          { return element_.empty(); }
    UInt size () const          { return element_.size();  }
    virtual void clear()        { element_.clear();        }

    typename Reference<T>::type      get(const UInt i);
    typename ConstReference<T>::type get(const UInt i) const;

    virtual Vector<T> add(const Vector<T>&);

private:
    std::vector<T> element_;
};

template<typename T>
class Vector<T*> : public VectorPtr<T> {
public:
    Vector() {}
    template<typename T2>
    Vector(T2* e)                     : VectorPtr<T>(e) {}
    template<typename T2>
    Vector(const std::vector<T2*>& e) : VectorPtr<T>(e) {}
    template<typename T2>
    Vector(VectorPtr<T2>&       rhs) : VectorPtr<T>(rhs) {}
    template<typename T2>
    Vector(const VectorPtr<T2>& rhs) : VectorPtr<T>(rhs) {}
    Vector(VectorPtr<T>&        rhs) : VectorPtr<T>(rhs) {}
    template<typename T2>
    Vector(VectorPtr<T2>&& rhs) : VectorPtr<T>(std::move(rhs)) {}
    Vector(VectorPtr<T >&& rhs)  : VectorPtr<T>(std::move(rhs)) {}
    virtual ~Vector() {}

    VectorPtr<T>& operator=(VectorPtr<T>&  rhs) {
        return VectorPtr<T>::operator=(rhs);
    }
    VectorPtr<T>& operator=(VectorPtr<T>&& rhs) {
        return VectorPtr<T>::operator=(std::move(rhs));
    }
};

#endif /* COMMON_BASE_SEQUENCE_VECTOR_H_ */
