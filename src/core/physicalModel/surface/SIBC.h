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

#include "math/matrix/Static.h"
#include "filesystem/Project.h"

#include "Surface.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

class SIBC : public virtual Surface {
public:
    typedef std::pair<std::complex<Math::Real>, Math::MatC22> PoleResidue;

    SIBC(const Id id,
            const std::string& name,
            const Math::MatC22& Zinfinite,
            const Math::MatC22& Zstatic,
            const std::vector<PoleResidue>& poleImpedance);
    virtual ~SIBC();

    SEMBA_CLASS_DEFINE_CLONE(SIBC);

    virtual std::size_t getNumberOfPoles() const;

    virtual void printInfo() const;

    std::vector<PoleResidue> getPoleZ() const {return poleZ_;}
    Math::MatC22 getZInfinity() const {return ZInfinity_;}
    Math::MatC22 getZLinear() const {return ZLinear_;}

private:
    Math::MatC22 ZInfinity_, ZLinear_;
    std::vector<PoleResidue> poleZ_;
};

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

