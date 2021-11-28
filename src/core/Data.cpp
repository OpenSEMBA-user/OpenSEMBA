#include "Data.h"
    
namespace SEMBA {

Data::Data(const Data& rhs) {

    solver = nullptr;
    mesh = nullptr;
    
    filename = rhs.filename;
    solver = rhs.solver;
    
    physicalModels = rhs.physicalModels;
    
    if (rhs.mesh != nullptr) {
        mesh = rhs.mesh->cloneTo<Geometry::Mesh::Mesh>();
        mesh->reassignPointers(physicalModels);
        
        outputRequests = rhs.outputRequests;
        for (size_t i = 0; i < outputRequests->size(); ++i) {
            (*outputRequests)(i)->reassignPointersInTarget(mesh->coords());
        }
        
        
        sources = rhs.sources->clone();
        for (size_t i = 0; i < sources->size(); ++i) {
            Geometry::Element::Group<const Geometry::Elem> sourceElems =
                    (*sources)(i)->elems();
            mesh->reassign(sourceElems);
            (*sources)(i)->set(sourceElems);
        }
    }
}

Data::~Data() {
    if (mesh != nullptr) {
        delete mesh;
    }
    if (sources != nullptr) {
        delete sources;
    }
    if (outputRequests != nullptr) {
        delete outputRequests;
    }
}

Data& Data::operator=(const Data& rhs) {
    solver = nullptr;
    mesh = nullptr;
    sources = nullptr;
    outputRequests = nullptr;

    filename = rhs.filename;
    solver = rhs.solver;
        
    physicalModels = rhs.physicalModels;
    
    if (rhs.mesh != nullptr) {
        mesh = rhs.mesh->cloneTo<Geometry::Mesh::Mesh>();
        mesh->reassignPointers(physicalModels);
    
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

} /* namespace SEMBA */
