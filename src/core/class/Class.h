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
#ifndef CLASSBASE_H_
#define CLASSBASE_H_

#include <cstdlib>
#include <memory>
#include <string>
#include <typeinfo>

class ClassBase : public std::enable_shared_from_this<ClassBase> {
public:
    ClassBase();
    virtual ~ClassBase();

    template<typename T>
    bool is() const;

    virtual ClassBase* clone  () const = 0;
    template<typename T>
    T*                 cloneTo() const;

    std::shared_ptr<      ClassBase> getSharedPtr();
    std::shared_ptr<const ClassBase> getSharedPtr() const;

    template<typename T>
    T*       castTo();
    template<typename T>
    const T* castTo() const;

    virtual void printInfo() const = 0;
};

#include "ClassBase.hpp"

#define DEFINE_CLONE(NAME)      \
    NAME* clone() const {       \
        return new NAME(*this); \
    }

#endif /* CLASSBASE_H_ */
