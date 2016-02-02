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
// File: physicalmodel.cpp
// =============== Includes headers ===========================================

#include "PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

PhysicalModel::PhysicalModel() {

}

PhysicalModel::PhysicalModel(const std::string& name) {
    name_ = name;
}

PhysicalModel::PhysicalModel(const PhysicalModel& rhs) {
    name_ = rhs.name_;
}

PhysicalModel::~PhysicalModel() {

}

const std::string& PhysicalModel::getName() const {
    return name_;
}
 
void PhysicalModel::setName(const std::string& newName) {
    name_ = newName;
}

void
PhysicalModel::printInfo() const {
    std::cout << " --- Physical Model Info ---" << std::endl;
    std::cout << "Id: " << getId() << std::endl;
    std::cout << "Name: " << name_ << std::endl;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
