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

#include <physicalModel/surface/Multilayer.h>
#include <vector>


namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

Multilayer::Multilayer(
        const Id id,
        const std::string& name,
        const std::vector<Math::Real>& thickness,
        const std::vector<Math::Real>& relPermittivity,
        const std::vector<Math::Real>& relPermeability,
        const std::vector<Math::Real>& elecCond)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    thickness_ = thickness;
    relPermittivity_ = relPermittivity;
    relPermeability_ = relPermeability;
    elecCond_ = elecCond;
    const std::size_t nLayers = thickness_.size();
    if (relPermittivity_.size() != nLayers ||
        relPermeability_.size() != nLayers ||
        elecCond_.size() != nLayers) {
        throw Error::SurfaceMultilayer::IncompatibleSizes();
    }
}

Multilayer::~Multilayer() {

}

std::size_t Multilayer::getNumberOfLayers() const {
    return thickness_.size();
}

std::string Multilayer::printLayer(const std::size_t i) const {
    assert(i < getNumberOfLayers());
    std::stringstream ss;
    ss << elecCond_[i] <<
            " " << relPermittivity_[i]*Math::Constants::eps0 <<
            " " << relPermeability_[i] * Math::Constants::mu0 <<
            " " << thickness_[i];
    return std::string(ss.str());
}

Math::Real Multilayer::getThickness(const std::size_t i) const {
    return thickness_[i];
}

Math::Real Multilayer::getPermittivity(const std::size_t i) const {
    return relPermittivity_[i] * Math::Constants::eps0;
}

Math::Real Multilayer::getPermeability(const std::size_t i) const {
    return relPermeability_[i] * Math::Constants::mu0;
}

Math::Real Multilayer::getElecCond(const std::size_t i) const {
    return elecCond_[i];
}

void Multilayer::printInfo() const {
    std::cout << " --- SurfaceMultilayer info ---" << std::endl;
    Surface::printInfo();
    std::cout << "Number of layers: " << getNumberOfLayers() << std::endl;
    std::cout << "#, Thickness, Permittivity, Permeability, ElecCond"
              << std::endl;
    for (std::size_t i = 0; i < getNumberOfLayers(); i++) {
        std::cout<< i << ": "
        << thickness_[i] << " "
        << relPermittivity_[i] << " "
        << relPermeability_[i] << " "
        << elecCond_[i] << std::endl;
    }
    std::cout << " --- End of SurfaceMultilayer info ---" << std::endl;
}

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
