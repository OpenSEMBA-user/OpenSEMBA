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
 * PortTEMCoaxial.h
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_
#define SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_

#include "PortTEM.h"

class PortTEMCoaxial : public PortTEM {
public:
    PortTEMCoaxial(
            Magnitude* magnitude,
            const GroupElements<const Surf>& elem,
            const ExcitationMode excMode,
            const CVecR3& origin,
            const Real innerRadius,
            const Real outerRadius);
    PortTEMCoaxial(const PortTEMCoaxial& rhs);
    virtual ~PortTEMCoaxial();

    DEFINE_CLONE(PortTEMCoaxial);

    void set(const GroupElements<const Elem>&);

    bool hasSameProperties(const PortTEMCoaxial& rhs) const;

    const string& getName() const;

    CVecR3 getOrigin(const BoundTerminations& sym = BoundTerminations()) const;
    CVecR3 getWeight(
            const CVecR3& pos,
            const BoundTerminations& sym = BoundTerminations()) const;

    void printInfo() const;
private:
    CVecR3 origin_;
    Real innerRadius_, outerRadius_;
};

#endif /* SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_ */
