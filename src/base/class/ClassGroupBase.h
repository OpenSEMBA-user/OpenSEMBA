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
 * ClassGroup.h
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_BASE_CLASSGROUPBASE_H_
#define SRC_COMMON_BASE_CLASSGROUPBASE_H_

#include <string>

#include "base/group/Group.h"

#include "ClassBase.h"

template<typename G>
class ClassGroupBase : public virtual ClassBase {
public:
    class ErrorEmpty : public Error {
    public:
        ErrorEmpty(const std::string&);
        ~ErrorEmpty() throw();
    };

    ClassGroupBase();
    virtual ~ClassGroupBase();

    bool check(const std::string& = "Group") const;

    virtual G elems() const = 0;

    virtual void set(const G&) = 0;
    virtual void add(const G&) = 0;
};

#include "ClassGroupBase.hpp"

#endif /* SRC_COMMON_BASE_CLASSGROUPBASE_H_ */
