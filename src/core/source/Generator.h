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
 * Generator.h
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_

#include "../sources/EMSource.h"

class Generator : public EMSource<Nod> {
public:
    typedef enum {
        voltage = 1,
        current = 2
    } Type;
    typedef enum {
        soft,
        hard
    } Hardness;

    Generator();
    Generator(const Magnitude* magnitude,
              GroupElements<Nod>& elem,
              const Type& generatorType,
              const Hardness& hardness);
    Generator(const Generator& rhs);
    virtual ~Generator();

    DEFINE_CLONE(Generator);

    bool hasSameProperties(const EMSourceBase& rhs) const;

    const string& getName() const;
    Type getType() const;

    void printInfo() const;

private:
    Type type_;
    Hardness hardness_;

    string getTypeStr() const;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_GENERATOR_H_ */
