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

#ifndef SEMBA_PHYSICALMODEL_WIREEXTREMES_H_
#define SEMBA_PHYSICALMODEL_WIREEXTREMES_H_

#include "Types.h"

#include "Wire.h"
#include "Multiport.h"

namespace SEMBA {
namespace PhysicalModel {

class WireExtremes: public Wire {
public:
    WireExtremes(const Wire& wire,
                   const Multiport* extremeL,
                   const Multiport* extremeR);
    WireExtremes(const WireExtremes& rhs);
    virtual ~WireExtremes();

    SEMBA_CLASS_DEFINE_CLONE(WireExtremes);

    const Multiport *getExtreme(const Size i) const { return extreme_[i]; }

    void setExtreme(const Size i, const Multiport* extreme);

    void printInfo() const;

private:
    const Multiport* extreme_[2];
};

} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_WIREEXTREMES_H_ */
