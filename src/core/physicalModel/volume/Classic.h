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

#pragma once

#include <limits>

#include "Volume.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

class Classic : public virtual Volume {
public:
    Classic(const Id matId,
                  const std::string& name,
                  const Math::Real relativePermittivity = 1.0,
                  const Math::Real relativePermeability = 1.0,
                  const Math::Real electricConductivity = 0.0,
                  const Math::Real magneticConductivity = 0.0);
    Classic(const Classic&);
    virtual ~Classic();

    SEMBA_CLASS_DEFINE_CLONE(Classic);

    Math::Real getRelativePermittivity() const;
    Math::Real getPermittivity() const;
    Math::Real getRelativePermeability() const;
    Math::Real getPermeability() const;
    Math::Real getImpedance() const;
    Math::Real getAdmitance() const;
    Math::Real getElectricConductivity() const;
    Math::Real getMagneticConductivity() const;
    bool isVacuum() const;

    void printInfo() const;
private:
    Math::Real rEps_; // Rel. permittivity @ infte. freq.
    Math::Real rMu_; // Rel. permeability @ infte. freq.
    Math::Real electricConductivity_;
    Math::Real magneticConudctivity_;
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

