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

#ifndef SEMBA_PHYSICALMODEL_SURFACESIBC_H_
#define SEMBA_PHYSICALMODEL_SURFACESIBC_H_

#include "math/matrix/Static.h"
#include "filesystem/Project.h"

#include "Surface.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

class SIBC : public virtual Surface {
public:
    SIBC();
    SIBC(const Id id,
                const std::string& name,
                const Math::MatR22& Zinfinite,
                const Math::MatR22& Zstatic,
                const std::vector<Math::Real>& pole,
                const std::vector<Math::MatR22>& Z);
    SIBC(const Id id,
                const std::string& name,
                const FileSystem::Project& file);
    SIBC(const SIBC& rhs);
    virtual ~SIBC();

    SEMBA_CLASS_DEFINE_CLONE(SIBC);

    virtual SIBC& operator=(const SIBC &param);
    void setZInfinity(const Math::MatR22& Zinf);
    void setZStatic(const Math::MatR22& Zsta);
    virtual void addPole(const Math::Real pole, const Math::MatR22& Z);
    virtual std::size_t getNumberOfPoles() const;
    virtual void printInfo() const;
    const FileSystem::Project getFile() const;

protected:
    Math::MatR22 ZInfinity_, ZStatic_;
    std::vector<Math::Real> pole_; // Impedance poles.
    std::vector<Math::MatR22> Z_; // Impedance residuals.
    FileSystem::Project file_;
};

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_SURFACESIBC_H_ */
