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

#ifndef SEMBA_PHYSICALMODEL_WIRE_H_
#define SEMBA_PHYSICALMODEL_WIRE_H_

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

class Wire : public virtual PhysicalModel {
public:
    Wire(const Id id,
         const std::string name,
         const Math::Real radius,
         const Math::Real resistance,
         const Math::Real inductance);
    Wire(const Wire&);
    virtual ~Wire();

    SEMBA_CLASS_DEFINE_CLONE(Wire);

    virtual Math::Real getRadius() const;
    virtual Math::Real getResistance() const;
    virtual Math::Real getInductance() const;
    virtual void printInfo() const;
private:
    Math::Real radius_;
    Math::Real resistance_; // Resistance per meter.
    Math::Real inductance_; // Inductance per meter.
};

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_WIRE_H_ */
