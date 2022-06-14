#include "DataExtended.h"

namespace SEMBA {

DataExtended::DataExtended(const DataExtended& rhs) {
    boundary = rhs.boundary;
    grids = rhs.grids;
    analysis = rhs.analysis;
    
    model = rhs.model;
    
    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        outputRequest->setTarget(
            model.unstructuredMesh.reassign(outputRequest->getTarget())
        );
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        source->setTarget(
            model.unstructuredMesh.reassign(source->getTarget())
        );
    }
}

DataExtended& DataExtended::operator=(const DataExtended& rhs) {
    boundary = rhs.boundary;
    grids = rhs.grids;
    analysis = rhs.analysis;

    model = rhs.model;

    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        outputRequest->setTarget(
            model.unstructuredMesh.reassign(outputRequest->getTarget())
        );
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        source->setTarget(
            model.unstructuredMesh.reassign(source->getTarget())
        );
    }

    return *this;
}
}
