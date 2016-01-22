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

#include <vector>

#include "SurfaceMultilayer.h"

namespace SEMBA {
namespace PhysicalModel {

SurfaceMultilayer::SurfaceMultilayer(
        const Id id,
        const std::string& name,
        const std::vector<Math::Real>& thickness,
        const std::vector<Math::Real>& relPermittivity,
        const std::vector<Math::Real>& relPermeability,
        const std::vector<Math::Real>& elecCond,
        const std::vector<Math::Real>& magnCond)
:   Surface(id, name) {
    thickness_ = thickness;
    relPermittivity_ = relPermittivity;
    relPermeability_ = relPermeability;
    elecCond_ = elecCond;
    magnCond_ = magnCond;
    const std::size_t nLayers = thickness_.size();
    if (relPermittivity_.size() != nLayers ||
        relPermeability_.size() != nLayers ||
        elecCond_.size() != nLayers ||
        magnCond_.size() != nLayers) {
        throw Error::SurfaceMultilayer::IncompatibleSizes();
    }
}

std::size_t SurfaceMultilayer::getNumberOfLayers() const {
    return thickness_.size();
}

std::string SurfaceMultilayer::printLayer(const std::size_t i) const {
    assert(i < getNumberOfLayers());
    std::stringstream ss;
    ss << elecCond_[i] << " " << relPermittivity_[i]*Math::Constants::eps0
       << " " << relPermeability_[i] * Math::Constants::mu0 << " "
       << magnCond_[i] << " " << thickness_[i];
    return std::string(ss.str());
}

Math::Real SurfaceMultilayer::getThickness(const std::size_t i) const {
    return thickness_[i];
}

Math::Real SurfaceMultilayer::getPermittivity(const std::size_t i) const {
    return relPermittivity_[i] * Math::Constants::eps0;
}

Math::Real SurfaceMultilayer::getPermeability(const std::size_t i) const {
    return relPermeability_[i] * Math::Constants::mu0;
}

Math::Real SurfaceMultilayer::getElecCond(const std::size_t i) const {
    return elecCond_[i];
}

Math::Real SurfaceMultilayer::getMagnCond(const std::size_t i) const {
    return magnCond_[i];
}

void SurfaceMultilayer::printInfo() const {
    std::cout << " --- SurfaceMultilayer info ---" << std::endl;
    Surface::printInfo();
    std::cout << "Number of layers: " << getNumberOfLayers() << std::endl;
    std::cout << "#, Thickness, Permittivity, Permeability, ElecCond, MagnCond"
              << std::endl;
    for (std::size_t i = 0; i < getNumberOfLayers(); i++) {
        std::cout<< i << ": "
        << thickness_[i] << " "
        << relPermittivity_[i] << " "
        << relPermeability_[i] << " "
        << elecCond_[i] << " "
        << magnCond_[i] << std::endl;
    }
    std::cout << " --- End of SurfaceMultilayer info ---" << std::endl;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
