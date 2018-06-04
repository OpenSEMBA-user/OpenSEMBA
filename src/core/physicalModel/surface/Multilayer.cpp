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
        const std::vector<Layer>& layers,
        const std::vector<FittingOptions>& options)
:   Identifiable<Id>(id),
    PhysicalModel(name),
    layers_(layers),
    options_(options) {

    if (options_.size() > 1) {
        throw std::runtime_error(
                "Multilayer Fitting options may contain up to one element");
    }
}

Multilayer::~Multilayer() {

}

std::string Multilayer::printLayer(const std::size_t i) const {
    assert(i < getNumberOfLayers());
    std::stringstream ss;
    ss << getElecCond(i) << " " << getPermittivity(i) <<
            " " << getPermeability(i) << " " << getThickness(i);
    return std::string(ss.str());
}

Math::Real Multilayer::getThickness(const std::size_t i) const {
    return layers_[i].getThickness();
}

Math::Real Multilayer::getPermittivity(const std::size_t i) const {
    return layers_[i].getRelPermittivity() * Math::Constants::eps0;
}

Math::Real Multilayer::getPermeability(const std::size_t i) const {
    return layers_[i].getRelPermeability() * Math::Constants::mu0;
}

Math::Real Multilayer::getElecCond(const std::size_t i) const {
    return layers_[i].getElecCond();
}

Math::Real Multilayer::getMagnCond(const std::size_t i) const {
    return 0.0;
}

bool Multilayer::hasFittingOptions() const {
    if (options_.size() == 1) {
        return true;
    } else {
        return false;
    }
}

Multilayer::FittingOptions Multilayer::getFittingOptions() const {
    if (options_.size() != 1) {
        throw std::runtime_error(
                "Trying to access a corrupt fitting options value");
    }
    return options_.front();
}

void Multilayer::printInfo() const {
    std::cout << " --- SurfaceMultilayer info ---" << std::endl;
    Surface::printInfo();
    std::cout << "Number of layers: " << getNumberOfLayers() << std::endl;
    std::cout << "#, Thickness, Permittivity, Permeability, ElecCond"
              << std::endl;
    for (std::size_t i = 0; i < getNumberOfLayers(); i++) {
        std::cout<< i << printLayer(i) << std::endl;
    }
    std::cout << " --- End of SurfaceMultilayer info ---" << std::endl;
}

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */


