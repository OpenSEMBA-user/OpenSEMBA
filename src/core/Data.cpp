#include "Data.h"
    
namespace SEMBA {

Data::Data(const Data& rhs) {

    solver = nullptr;
    mesh = nullptr;
    
    filename = rhs.filename;
    solver = rhs.solver;
    
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
        auto sourceElems = source->getTarget();
        source->setTarget(mesh->reassign(sourceElems));
    }
}

Data& Data::operator=(const Data& rhs) {
    solver = nullptr;
    mesh = nullptr;

    filename = rhs.filename;
    solver = rhs.solver;
        
    physicalModels = rhs.physicalModels;
    
    if (rhs.mesh == nullptr) {        
        return *this;
    }

    mesh = rhs.mesh->clone();
    mesh->reassignPointers(physicalModels);

    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        auto outRqElems = outputRequest->getTarget();
        outputRequest->setTarget(mesh->reassign(outRqElems));
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        auto sourceElems = source->getTarget();
        source->setTarget(mesh->reassign(sourceElems));
    }

    return *this;
}

} /* namespace SEMBA */
