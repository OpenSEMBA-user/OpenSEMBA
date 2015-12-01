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

#include "VolumeDispersive.h"

namespace SEMBA {
namespace PhysicalModel {

VolumeDispersive::VolumeDispersive(const Id id,
                                   const std::string& name,
                                   const Math::Real rEpsInfty,
                                   const Math::Real rMuInfty,
                                   const Math::Real elecCond,
                                   const Math::Real magnCond)
:   Volume(id, name) {
    rEpsInfty_ = rEpsInfty;
    rMuInfty_ = rMuInfty;
    // Adds conductivity as a permittivity pole.
    if (elecCond != 0.0) {
        std::complex<Math::Real> pole(0.0);
        std::complex<Math::Real> residue(elecCond/Math::Real(2.0)/
                                         Math::Constants::eps0, 0);
        poleResidue_.push_back(PoleResidue(pole,residue));
    }
    //
    if (magnCond != 0.0) {
        throw Error::VolumeDispersive::MagneticMaterial();
    }
}

VolumeDispersive::VolumeDispersive(const Id id,
                                   const std::string& name,
                                   const Math::Real rEps,
                                   const Math::Real rMu,
                                   const Math::Real elecCond,
                                   const Math::Real magnCond,
                                   const std::vector<PoleResidue>& poleResidue)
:   Volume(id, name) {
    *this = VolumeDispersive(id, name, rEps, rMu, elecCond, magnCond);
    poleResidue_ = poleResidue;
}

VolumeDispersive::VolumeDispersive(const Id id,
                                   const std::string& name,
                                   const FileSystem::Project& file)
:   Volume(id, name) {
    rEpsInfty_ = 1.0;
    rMuInfty_ = 1.0;
    file_ = file;
}

VolumeDispersive::~VolumeDispersive() {

}

Size VolumeDispersive::getPoleNumber() const {
    return poleResidue_.size();
}

std::complex<Math::Real> VolumeDispersive::getPole(Size p) const {
    return poleResidue_[p].first;
}

std::complex<Math::Real> VolumeDispersive::getResidue(Size p) const {
    return poleResidue_[p].second;
}

bool VolumeDispersive::isDispersive() const {
    if (poleResidue_.size() > 0) {
        return true;
    }
    return false;
}

bool VolumeDispersive::isClassic() const {
    return isSimplyConductive();
}

bool VolumeDispersive::isSimplyConductive() const {
    if (!file_.empty()) {
        return false;
    }
    return (poleResidue_.size() <= 1 && std::abs(getPole(0)) == 0);
}

Math::Real VolumeDispersive::getElectricConductivity() const {
    if (getPoleNumber() > 1) {
        std::cout << std::endl << "WARNING @ getElectricConductivity: "
                << "This material is dispersive and its effective permittivity "
                << "depends on several parameters."
                << "Returning static limit conductivity." << std::endl;
    }
    for (Size i = 0; i < getPoleNumber(); i++) {
        if (std::abs(getPole(i)) == 0) {
            return getResidue(i).real() * 2.0 * Math::Constants::eps0;
        }
    }
    return 0.0;
}

void VolumeDispersive::addPole(
        const std::complex<Math::Real>& pole,
        const std::complex<Math::Real>& res) {
    poleResidue_.push_back(PoleResidue(pole,res));
    return;
}

const FileSystem::Project VolumeDispersive::getFile() const {
    return file_;
}

void VolumeDispersive::printInfo() const {
    std::cout << "--- VolumeDispersive info ---" << std::endl;
    Volume::printInfo();
    std::cout << "Type: " << "Dispersive material" << std::endl;
    std::cout << "Number of pole residue pairs: " << poleResidue_.size()
              << std::endl;
    std::cout << "# " << " re_a " << " im_a " << " re_c " << " im_c "
              << std::endl;
    for (Size i = 0; i < poleResidue_.size(); i++) {
        std::cout << i << " " << getPole(i).real() << " " << getPole(i).imag()
                  << " " << getResidue(i).real() << " " << getResidue(i).imag()
                  << std::endl;
    }
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
