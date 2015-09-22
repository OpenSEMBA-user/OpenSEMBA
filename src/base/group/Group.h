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
#ifndef GROUP_H_
#define GROUP_H_

#include <algorithm>
#include <iostream>
#include <typeinfo>

#include "../class/ClassBase.h"
#include "../sequence/Vector.h"

template<typename T>
class Group : public virtual ClassBase,
              public Vector<T*> {
public:
    Group() {}
    template<typename T2>
    Group(T2* e)                     : Vector<T*>(e) {}
    template<typename T2>
    Group(const std::vector<T2*>& e) : Vector<T*>(e) {}
    template<typename T2>
    Group(VectorPtr<T2>&       rhs) : Vector<T*>(rhs) {}
    template<typename T2>
    Group(const VectorPtr<T2>& rhs) : Vector<T*>(rhs) {}
    Group(VectorPtr<T>&        rhs) : Vector<T*>(rhs) {}
    template<typename T2>
    Group(VectorPtr<T2>&& rhs) : Vector<T*>(std::move(rhs)) {}
    Group(VectorPtr<T >&& rhs) : Vector<T*>(std::move(rhs)) {}
    virtual ~Group() {}

    Group<T>* clone() const;

    virtual Group& operator=(VectorPtr<T>&  rhs);
    virtual Group& operator=(VectorPtr<T>&& rhs);

    template<class T2>
    bool emptyOf() const;
    template<class T2>
    UInt sizeOf () const;

    template<class T2>
    Group<typename std::conditional<std::is_const<T>::value,
                                    const T2, T2>::type>
                    getOf    ();
    template<class T2>
    Group<const T2> getOf    () const;
    template<class T2>
    Group<typename std::conditional<std::is_const<T>::value,
                                    const T2, T2>::type>
                    getOfOnly();
    template<class T2>
    Group<const T2> getOfOnly() const;

    void printInfo() const;

private:
    template<typename T2>
    std::vector<UInt> getElemsOf_       () const;
    template<typename T2>
    std::vector<UInt> getElemsNotOf_    () const;
    template<typename T2>
    std::vector<UInt> getElemsOfOnly_   () const;
    template<typename T2>
    std::vector<UInt> getElemsNotOfOnly_() const;
};

#include "Group.hpp"

#define DEFINE_GROUP_CLONE(NAME, T)             \
    NAME<T>* clone() const {                    \
        return new NAME<T>(this->cloneElems()); \
    }

#endif /* GROUP_H_ */
