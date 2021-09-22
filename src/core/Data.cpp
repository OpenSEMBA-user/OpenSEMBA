

#include "Data.h"

namespace SEMBA {

Data::Data() {
    solver = nullptr;
    mesh = nullptr;
    physicalModels = nullptr;
    sources = nullptr;
    outputRequests = nullptr;
}

Data::Data(const Data& rhs) {

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
