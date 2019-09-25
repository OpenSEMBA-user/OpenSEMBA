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

#include <physicalModel/volume/Dispersive.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

Dispersive::Dispersive(const Id id,
                                   const std::string& name,
                                   const Math::Real rEps,
                                   const Math::Real rMu,
                                   const Math::Real elecCond,
                                   const Math::Real magnCond,
                                   const std::vector<PoleResidue>& poleResidue)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    rEpsInfty_ = rEps;
    rMuInfty_ = rMu;
    // Adds conductivity as a permittivity pole.
    if (elecCond != 0.0) {
        std::complex<Math::Real> pole(0.0);
        std::complex<Math::Real> residue(elecCond / Math::Real(2.0) /
                                         Math::Constants::eps0, 0);
        poleResidue_.push_back(PoleResidue(pole, residue));
    }
    //
    if (magnCond != 0.0) {
        throw std::logic_error("Dispersive magnetic materials not implemented");
    }
    poleResidue_ = poleResidue;
}

Dispersive::Dispersive(const Id id,
                       const std::string& name,
                       const FileSystem::Project& file)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
	rEpsInfty_ = 1.0;
    rMuInfty_ = 1.0;
	if (!file.canOpen()) {
		throw std::ios_base::failure(std::string("File: ") + file +
			std::string(" does not exist"));
	}
    file_ = file;
}

Dispersive::Dispersive(const Dispersive& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    rEpsInfty_ = rhs.rEpsInfty_;
    rMuInfty_ = rhs.rMuInfty_;
    poleResidue_ = rhs.poleResidue_;
    file_ = rhs.file_;
}

Dispersive::~Dispersive() {

}

std::size_t Dispersive::getPoleNumber() const {
    return poleResidue_.size();
}

std::complex<Math::Real> Dispersive::getPole(std::size_t p) const {
    return poleResidue_[p].first;
}

std::complex<Math::Real> Dispersive::getResidue(std::size_t p) const {
    return poleResidue_[p].second;
}

bool Dispersive::isDispersive() const {
    if (poleResidue_.size() > 0) {
        return true;
    }
    return false;
}

bool Dispersive::isClassic() const {
    return isSimplyConductive();
}

bool Dispersive::isSimplyConductive() const {
    if (!file_.empty()) {
        return false;
    }
    return (poleResidue_.size() <= 1 && std::abs(getPole(0)) == 0);
}

Math::Real Dispersive::getElectricConductivity() const {
    if (getPoleNumber() > 1) {
        std::cout << std::endl << "WARNING @ getElectricConductivity: "
                  << "This material is dispersive and its effective "
                  << "permittivity depends on several parameters. "
                  << "Returning static limit conductivity." << std::endl;
    }
    for (std::size_t i = 0; i < getPoleNumber(); i++) {
        if (std::abs(getPole(i)) == 0) {
            return getResidue(i).real() * 2.0 * Math::Constants::eps0;
        }
    }
    return 0.0;
}

void Dispersive::addPole(
        const std::complex<Math::Real>& pole,
        const std::complex<Math::Real>& res) {
    poleResidue_.push_back(PoleResidue(pole,res));
    return;
}

const FileSystem::Project Dispersive::getFile() const {
    return file_;
}

void Dispersive::printInfo() const {
    std::cout << "--- VolumeDispersive info ---" << std::endl;
    Volume::printInfo();
    std::cout << "Type: " << "Dispersive material" << std::endl;
    std::cout << "Number of pole residue pairs: " << poleResidue_.size()
              << std::endl;
    std::cout << "# " << " re_a " << " im_a " << " re_c " << " im_c "
              << std::endl;
    for (std::size_t i = 0; i < poleResidue_.size(); i++) {
        std::cout << i << " " << getPole(i).real() << " " << getPole(i).imag()
                  << " " << getResidue(i).real() << " " << getResidue(i).imag()
                  << std::endl;
    }
}

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
