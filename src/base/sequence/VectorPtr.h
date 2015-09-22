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
 * VectorPtr.h
 *
 *  Created on: 23/6/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_SEQUENCE_VECTORPTR_H_
#define COMMON_BASE_SEQUENCE_VECTORPTR_H_

#include <set>

#include "Sequence.h"

template<typename T>
class VectorPtr : public Sequence<T*> {
public:
    VectorPtr();
    template<typename T2>
    VectorPtr(T2*);
    template<typename T2>
    VectorPtr(const std::vector<T2*>&);
    template<typename T2>
    VectorPtr(VectorPtr<T2>&);
    template<typename T2>
    VectorPtr(VectorPtr<T2>&&);
    template<typename T2>
    VectorPtr(const VectorPtr<T2>&);
    VectorPtr(VectorPtr&);
    VectorPtr(VectorPtr&&);
    virtual ~VectorPtr();

    VectorPtr<typename std::remove_const<T>::type> cloneElems() const;

    virtual VectorPtr& operator=(VectorPtr&  rhs);
    virtual VectorPtr& operator=(VectorPtr&& rhs);

    void resize(const UInt nS)  { element_.resize (nS);    }
    void reserve(const UInt nS) { element_.reserve(nS);    }
    bool empty() const          { return element_.empty(); }
    UInt size () const          { return element_.size();  }
    virtual void clear()        { element_.clear();        }

    virtual typename Reference<T*>::type      get(const UInt i);
    virtual typename ConstReference<T*>::type get(const UInt i) const;
    VectorPtr<T>       get(const std::vector<UInt>&);
    VectorPtr<const T> get(const std::vector<UInt>&) const;

    virtual void set(const UInt i, T*);

    template<typename T2>
    VectorPtr add(T2*);
    template<typename T2>
    VectorPtr add(const std::vector<T2*>&);
    template<typename T2>
    VectorPtr add(VectorPtr<T2>&);
    template<typename T2, typename T3 = T,
             typename = typename std::enable_if<std::is_const<T3>::value>::type>
    VectorPtr add(const VectorPtr<T2>&);
    virtual VectorPtr add(VectorPtr&);
    virtual VectorPtr add(VectorPtr&&);

    virtual void remove(const UInt&);
    virtual void remove(const std::vector<UInt>&);

private:
    std::vector<std::shared_ptr<T>> element_;
};

#include "VectorPtr.hpp"

#endif /* COMMON_BASE_SEQUENCE_VECTORPTR_H_ */
