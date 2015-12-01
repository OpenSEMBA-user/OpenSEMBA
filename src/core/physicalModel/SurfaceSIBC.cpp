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

#include "SurfaceSIBC.h"

namespace SEMBA {
namespace PhysicalModel {

SurfaceSIBC::SurfaceSIBC(const Id id,
                         const std::string& name,
                         const Math::MatR22& Zinfinite,
                         const Math::MatR22& Zstatic,
                         const std::vector<Math::Real>& pole,
                         const std::vector<Math::MatR22 >& Z)
:   Surface(id, name) {
    ZInfinity_ = Zinfinite;
    ZStatic_ = Zstatic;
    pole_ = pole;
    Z_ = Z;
}

SurfaceSIBC::SurfaceSIBC(const Id id,
                         const std::string& name,
                         const FileSystem::Project& file)
:   Surface(id, name) {
    file_ = file;
}

SurfaceSIBC::~SurfaceSIBC() {

}

SurfaceSIBC& SurfaceSIBC::operator=(const SurfaceSIBC &rhs) {
    if (this == &rhs) {
        return *this;
    }
    Surface::operator=(rhs);
    ZInfinity_ = rhs.ZInfinity_;
    ZStatic_ = rhs.ZStatic_;
    pole_ = rhs.pole_;
    Z_ = rhs.Z_;
    file_ = rhs.file_;
    return *this;
}

void SurfaceSIBC::setZInfinity(const Math::MatR22& ZInf) {
    ZInfinity_ = ZInf;
}

void SurfaceSIBC::setZStatic(const Math::MatR22& ZSta) {
    ZStatic_ = ZSta;
}

void SurfaceSIBC::addPole(const Math::Real vP, const Math::MatR22& Z) {
    pole_.push_back(vP);
    Z_.push_back(Z);
}

void SurfaceSIBC::printInfo() const {
    std::cout << "--- Physical model information ---"   << std::endl;
    std::cout << "Surface Impedance Boundary Condition" << std::endl;
    Surface::printInfo();
    std::cout << "Impedance @ infininite freq.:";
    ZInfinity_.printInfo();
    std::cout << "Impedance @ zero freq.:";
    ZStatic_.printInfo();
    if (pole_.size()) {
        std::cout << "Number of poles: " << pole_.size() << std::endl;
        std::cout << "# Pole Z11 Z12 Z21 Z22 " << std::endl;
    }
    for (register Size i = 0; i < pole_.size(); i++) {
        std::cout << i << " "
                << pole_[i] << " "
                << Z_[i](0,0) << " " << Z_[i](0,1) << " "
                << Z_[i](1,0) << " " << Z_[i](1,1) << std::endl;
    }
}

Size SurfaceSIBC::getNumberOfPoles() const {
    return pole_.size();
}

const FileSystem::Project SurfaceSIBC::getFile() const {
    return file_;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
