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

#include "Bound.h"

#include "PEC.h"
#include "PMC.h"
#include "PML.h"
#include "Periodic.h"
#include "Mur1.h"
#include "Mur2.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

Bound::Bound() {

}

Bound::~Bound() {

}

const Bound* strToBoundType(std::string str) {
    if (str.compare("PEC") == 0) {
        return new PEC(Id(0));
    } else if (str.compare("PMC") == 0) {
        return new PMC(Id(0));
    } else if (str.compare("PML") == 0) {
        return new PML(Id(0));
    } else if (str.compare("Periodic") == 0) {
        return new Periodic(Id(0));
    } else if (str.compare("MUR1") == 0) {
        return new Mur1(Id(0));
    } else if (str.compare("MUR2") == 0) {
        return new Mur2(Id(0));
    } else {
        throw std::logic_error("Unrecognized bound label: " + str);
    }
}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
