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

#ifndef SEMBA_PHYSICALMODEL_VOLUMEDISPERSIVE_H_
#define SEMBA_PHYSICALMODEL_VOLUMEDISPERSIVE_H_

#include <complex>
#include <exception>
#include <utility>

#include "filesystem/Project.h"

#include "Volume.h"

namespace SEMBA {
namespace PhysicalModel {

typedef std::pair<std::complex<Math::Real>,
                  std::complex<Math::Real>> PoleResidue;

class VolumeDispersive : public virtual Volume {
public:
    VolumeDispersive(const Id id,
                     const std::string& name,
                     const Math::Real rEps,
                     const Math::Real rMu,
                     const Math::Real elecCond,
                     const Math::Real magnCond,
                     const std::vector<PoleResidue>& poleResidue =
                        std::vector<PoleResidue>());
    VolumeDispersive(const Id id,
                     const std::string& name,
                     const FileSystem::Project& file);
    VolumeDispersive(const VolumeDispersive& rhs);
    virtual ~VolumeDispersive();

    SEMBA_CLASS_DEFINE_CLONE(VolumeDispersive);

    std::size_t getPoleNumber() const;
    std::complex<Math::Real> getPole(std::size_t p) const;
    std::complex<Math::Real> getResidue(std::size_t p) const;
    virtual Math::Real getElectricConductivity() const;

    bool isClassic() const;
    bool isSimplyConductive() const;
    bool isDispersive() const;

    void printInfo() const;
    const FileSystem::Project getFile() const;

protected:
    Math::Real rEpsInfty_, rMuInfty_; // @ InftyFreq.
    std::vector<PoleResidue> poleResidue_; // Residues for dispers model. c_p.
    FileSystem::Project file_;
    void addPole(const std::complex<Math::Real>& pole_,
                 const std::complex<Math::Real>& res_);
};

namespace Error {
namespace VolumeDispersive {

class MagneticMaterial : public Error {
public:
    MagneticMaterial() {}
    virtual ~MagneticMaterial() throw() {}

    const char* what() const throw() {
        return ("VolumeDispersive: "
                "Magnetic conductive materials are not supported.");
    }
};

} /* namespace VolumeDispersive */
} /* namespace Error */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_VOLUMEDISPERSIVE_H_ */
