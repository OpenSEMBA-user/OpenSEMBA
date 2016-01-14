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

#include "Boundary.h"

namespace SEMBA {
namespace Mesher {
namespace OpenFOAM {

Boundary::Boundary() {
    nFaces_ = 0;
    startFace_ = 0;
}

Boundary::Boundary(
        const std::string name,
        const std::size_t nFaces,
        const std::size_t startFace) : Layer(Geometry::LayerId(0), name) {
    nFaces_ = nFaces;
    startFace_ = startFace;
    if (isMaterial()) {
        setId(getLayerIdFromName());
    }
}

std::size_t Boundary::getFaces() const {
    return nFaces_;
}

std::size_t Boundary::getStartFace() const {
    return startFace_;
}

void Boundary::printInfo() const {
    std::cout << " --- OpenFoam boundary info ---" << std::endl;
    Layer::printInfo();
    std::cout << "Nfaces: " << nFaces_ << std::endl;
    std::cout << "StartFace: " << startFace_ << std::endl;
}

bool Boundary::isMaterial() const {
    if (getName().find("mat.") != getName().npos) {
        return true;
    }
    return false;
}

MatId Boundary::getMaterialIdFromName() const {
    assert(isMaterial());
    int firstDot = strpos(getName().c_str(), ".", 1);
    int secondDot = strpos(getName().c_str(), ".", 2);
    std::string values = getName().substr(firstDot + 1, secondDot);
    std::size_t res = atoi(values.c_str());
    return MatId(res);
}

Boundary::~Boundary() {
}

Geometry::LayerId Boundary::getLayerIdFromName() const {
    assert(isMaterial());
    int secondDot = strpos(getName().c_str(), ".", 2);
    std::string value = getName().substr(secondDot + 1, getName().npos);
    std::size_t res = atoi(value.c_str());
    return Geometry::LayerId(res);
}

int Boundary::strpos(const char *haystack, const char *needle, int nth) const {
    const char *res = haystack;
    for (int i = 1; i <= nth; i++)
    {
        res = strstr(res, needle);
        if (!res) {
            return -1;
        } else if (i != nth) {
            res++;
        }
    }
    return res - haystack;
}

}
}
}
