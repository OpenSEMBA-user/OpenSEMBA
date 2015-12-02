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

#include "Data.h"

namespace SEMBA {

Data::Data() {
    mesh = NULL;
    physicalModels = NULL;
    sources = NULL;
    outputRequests = NULL;
}

Data::Data(const Data& rhs)
:   FileSystem::Project(rhs) {
    mesh = NULL;
    physicalModels = NULL;
    sources = NULL;
    outputRequests = NULL;
    if (rhs.mesh != NULL) {
        mesh = rhs.mesh->cloneTo<Geometry::Mesh::Mesh>();
    }
    if (rhs.physicalModels != NULL) {
        physicalModels = rhs.physicalModels->clone();
    }
    if (rhs.sources != NULL) {
        sources = rhs.sources->clone();
    }
    if (rhs.outputRequests != NULL) {
        outputRequests = rhs.outputRequests->clone();
    }
}

Data::~Data() {
    if (mesh != NULL) {
        delete mesh;
    }
    if (physicalModels != NULL) {
        delete physicalModels;
    }
    if (sources != NULL) {
        delete sources;
    }
    if (outputRequests != NULL) {
        delete outputRequests;
    }
}

Data& Data::operator=(const Data& rhs) {
    if (this == &rhs) {
        return *this;
    }
    FileSystem::Project::operator=(rhs);
    mesh = NULL;
    physicalModels = NULL;
    sources = NULL;
    outputRequests = NULL;
    if (rhs.mesh != NULL) {
        mesh = rhs.mesh->cloneTo<Geometry::Mesh::Mesh>();
    }
    if (rhs.physicalModels != NULL) {
        physicalModels = rhs.physicalModels->clone();
    }
    if (rhs.sources != NULL) {
        sources = rhs.sources->clone();
    }
    if (rhs.outputRequests != NULL) {
        outputRequests = rhs.outputRequests->clone();
    }
    return *this;
}

void Data::printInfo() const {
    std::cout << " --- SEMBA data --- " << std::endl;
    if (mesh != NULL) {
        mesh->printInfo();
    } else {
        std::cout << "No info about mesh." << std::endl;
    }
    if (physicalModels != NULL) {
        physicalModels->printInfo();
    } else {
        std::cout << "No info about physical models." << std::endl;
    }
    if (sources != NULL) {
        sources->printInfo();
    } else {
        std::cout << "No info about electromagnetic sources." << std::endl;
    }
    if (outputRequests != NULL) {
        outputRequests->printInfo();
    } else {
        std::cout << "No info about output requests." << std::endl;
    }
}

bool Data::check() const {
    bool res = true;
    if (sources != NULL) {
//        res &= sources->check();
    }
    if (outputRequests != NULL) {
//        res &= outputRequests->check();
    }
    return res;
}

} /* namespace SEMBA */
