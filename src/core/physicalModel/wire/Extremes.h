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

#include "Wire.h"
#include "physicalModel/multiport/Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

class Extremes : public virtual Wire {
public:
    Extremes(const Wire& wire,
                 const Multiport::Multiport* extremeL,
                 const Multiport::Multiport* extremeR);
    Extremes(const Extremes& rhs);
    virtual ~Extremes();

    SEMBA_CLASS_DEFINE_CLONE(Extremes);

    const Multiport::Multiport *getExtreme(const std::size_t i) const {
        return extreme_[i];
    }

    void setExtreme(const std::size_t i, const Multiport::Multiport* extreme);

    void printInfo() const;

private:
    const Multiport::Multiport* extreme_[2];
};

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_WIREEXTREMES_H_ */
