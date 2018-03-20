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
    solver = nullptr;
    mesh = nullptr;
    physicalModels = nullptr;
    sources = nullptr;
    outputRequests = nullptr;
}

Data::Data(const Data& rhs)
:   FileSystem::Project(rhs) {

    solver = nullptr;
    mesh = nullptr;
    physicalModels = nullptr;
    sources = nullptr;
    outputRequests = nullptr;

    if (rhs.solver != nullptr) {
        solver = new Solver::Info(*rhs.solver);
    }

    if (rhs.physicalModels != nullptr) {
        physicalModels = rhs.physicalModels->clone();
    }

    if (rhs.mesh != nullptr) {
        mesh = rhs.mesh->cloneTo<Geometry::Mesh::Mesh>();
        if (physicalModels != nullptr) {
            mesh->reassignPointers(*physicalModels);
        } else {
            mesh->reassignPointers();
        }

        if (rhs.outputRequests != nullptr) {
            outputRequests = rhs.outputRequests->clone();
            for (size_t i = 0; i < outputRequests->size(); ++i) {
                Geometry::Element::Group<const Geometry::Elem> outRqElems =
                        (*outputRequests)(i)->elems();
                mesh->reassign(outRqElems);
                (*outputRequests)(i)->set(outRqElems);
            }
        }

        if (rhs.sources != nullptr) {
            sources = rhs.sources->clone();
            for (size_t i = 0; i < sources->size(); ++i) {
                Geometry::Element::Group<const Geometry::Elem> sourceElems =
                        (*sources)(i)->elems();
                mesh->reassign(sourceElems);
                (*sources)(i)->set(sourceElems);
            }
        }
    }

}

Data::~Data() {
    if (solver != nullptr) {
        delete solver;
    }
    if (mesh != nullptr) {
        delete mesh;
    }
    if (physicalModels != nullptr) {
        delete physicalModels;
    }
    if (sources != nullptr) {
        delete sources;
    }
    if (outputRequests != nullptr) {
        delete outputRequests;
    }
}

Data& Data::operator=(const Data& rhs) {
    if (this == &rhs) {
        return *this;
    }
    FileSystem::Project::operator=(rhs);
    solver = nullptr;
    mesh = nullptr;
    physicalModels = nullptr;
    sources = nullptr;
    outputRequests = nullptr;

    if (rhs.solver != nullptr) {
        solver = new Solver::Info(*rhs.solver);
    }

    if (rhs.physicalModels != nullptr) {
        physicalModels = rhs.physicalModels->clone();
    }

    if (rhs.mesh != nullptr) {
        mesh = rhs.mesh->cloneTo<Geometry::Mesh::Mesh>();
        if (physicalModels != nullptr) {
            mesh->reassignPointers(*physicalModels);
        } else {
            mesh->reassignPointers();
        }

        if (rhs.outputRequests != nullptr) {
            outputRequests = rhs.outputRequests->clone();
            for (size_t i = 0; i < outputRequests->size(); ++i) {
                Geometry::Element::Group<const Geometry::Elem> outRqElems =
                        (*outputRequests)(i)->elems();
                mesh->reassign(outRqElems);
                (*outputRequests)(i)->set(outRqElems);
            }
        }

        if (rhs.sources != nullptr) {
            sources = rhs.sources->clone();
            for (size_t i = 0; i < sources->size(); ++i) {
                Geometry::Element::Group<const Geometry::Elem> sourceElems =
                        (*sources)(i)->elems();
                mesh->reassign(sourceElems);
                (*sources)(i)->set(sourceElems);
            }
        }
    }

    return *this;
}

void Data::printInfo() const {
    std::cout << " --- SEMBA data --- " << std::endl;
    if (solver != nullptr) {
        solver->printInfo();
    }
    else {
        std::cout << "No info about solver options." << std::endl;
    }
    if (mesh != nullptr) {
        mesh->printInfo();
    } else {
        std::cout << "No info about mesh." << std::endl;
    }
    if (physicalModels != nullptr) {
        physicalModels->printInfo();
    } else {
        std::cout << "No info about physical models." << std::endl;
    }
    if (sources != nullptr) {
        sources->printInfo();
    } else {
        std::cout << "No info about sources." << std::endl;
    }
    if (outputRequests != nullptr) {
        outputRequests->printInfo();
    } else {
        std::cout << "No info about output requests." << std::endl;
    }
}

} /* namespace SEMBA */
