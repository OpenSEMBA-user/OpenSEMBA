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
 * Dipole.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef DIPOLE_H_
#define DIPOLE_H_

#include "../sources/EMSource.h"

class Dipole : public EMSource<Vol> {
public:
    Dipole(const Magnitude* magnitude,
           const GroupElements<Vol>& elem,
           Real   length,
           CVecR3 orientation,
           CVecR3 position);
    Dipole(const Dipole& rhs);
    virtual ~Dipole();

    DEFINE_CLONE(Dipole);

    bool hasSameProperties(const EMSourceBase& rhs) const;

    const string& getName() const;
    void printInfo() const;
protected:
    Real length_;
    CVecR3 orientation_;
    CVecR3 position_;
    Real gaussDelay_;
    Real spreadSqrt2_;
};

#endif /* DIPOLE_H_ */
