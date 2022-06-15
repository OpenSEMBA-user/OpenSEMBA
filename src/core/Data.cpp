#include "Data.h"
    
namespace SEMBA {

Data::Data(const Data& rhs) {
    solver = nullptr;
    mesh = nullptr;
    
    filename = rhs.filename;
    solver = rhs.solver;
    grids = rhs.grids;
    
    physicalModels = rhs.physicalModels;
    
    if (rhs.mesh == nullptr) {
        return;
    }

    mesh = rhs.mesh->clone();
    mesh->reassignPointers(physicalModels);

    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        outputRequest->setTarget(
            mesh->reassign(outputRequest->getTarget())
        );
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        source->setTarget(
            mesh->reassign(source->getTarget())
        );
    }
}

Data& Data::operator=(const Data& rhs) {
    solver = nullptr;
    mesh = nullptr;

    filename = rhs.filename;
    solver = rhs.solver;
    grids = rhs.grids;
        
    physicalModels = rhs.physicalModels;
    
    if (rhs.mesh == nullptr) {        
        return *this;
    }

    mesh = rhs.mesh->clone();
    mesh->reassignPointers(physicalModels);

    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        outputRequest->setTarget(
            mesh->reassign(outputRequest->getTarget())
        );
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        source->setTarget(
            mesh->reassign(source->getTarget())
        );
    }

    return *this;
}

} /* namespace SEMBA */
