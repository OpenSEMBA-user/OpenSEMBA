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
 * WaveportRectangular.h
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_SOURCES_PORTWAVEGUIDERECTANGULAR_H_
#define SRC_COMMON_SOURCES_PORTWAVEGUIDERECTANGULAR_H_

#include "PortWaveguide.h"

class PortWaveguideRectangular: public PortWaveguide {
public:
    PortWaveguideRectangular(
            Magnitude* magnitude,
            const GroupElements<const Surf>& elem,
            const ExcitationMode excMode,
            const pair<UInt,UInt> mode);
    PortWaveguideRectangular(const PortWaveguideRectangular&);
    virtual ~PortWaveguideRectangular();

    DEFINE_CLONE(PortWaveguideRectangular);
    bool hasSameProperties(const EMSourceBase& rhs) const;

    void set(const GroupElements<const Elem>&);

    const string& getName() const;
    Real getWidth(const BoundTerminations& sym) const;
    Real getHeight(const BoundTerminations& sym) const;

    CVecR3 getOrigin(const BoundTerminations& sym) const;
    CVecR3 getWeight(
            const CVecR3& pos,
            const BoundTerminations& sym) const;
private:
    BoxR3 box_; // This has been included for performance.
};

#endif /* SRC_COMMON_SOURCES_PORTWAVEGUIDERECTANGULAR_H_ */
