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

#ifndef SEMBA_PHYSICALMODEL_MULTIPORT_H_
#define SEMBA_PHYSICALMODEL_MULTIPORT_H_

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

class Multiport : public virtual PhysicalModel {
public:
    enum Type {
        // Predefined
        shortCircuit = 1,
        openCircuit = 2,
        matched = 3,
        // RLC types.
        sRLC = 11,    // R+L+C.
        sRLpC = 12,   // (R + L)//C.
        sRCpL = 13,   // (R + C)//L.
        sRpLC = 14,   // R + (L//C).
        sLCpR = 15,   // (L + C)//R.
        sLpRC = 16,   // L + (R//C).
        sCpLR = 17,   // C + (L//R).
        pRLC = 18,     // R//L//C.
        // Dispersive
        dispersive = 100,
        undefined = 0
    };
    Multiport();
    virtual ~Multiport();

    virtual Type getType() const;

protected:
    Type type_;
    std::string getTypeStr() const;
};

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_MULTIPORT_H_ */
