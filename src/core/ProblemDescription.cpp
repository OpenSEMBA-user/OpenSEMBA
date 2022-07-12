#include "ProblemDescription.h"

namespace SEMBA {

ProblemDescription::ProblemDescription(const ProblemDescription& rhs) {
    grids = rhs.grids;
    analysis = rhs.analysis;
    
    model = rhs.model;
    
    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        outputRequest->setTarget(
            model.mesh.reassign(outputRequest->getTarget())
        );
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        source->setTarget(
            model.mesh.reassign(source->getTarget())
        );
    }
}

ProblemDescription& ProblemDescription::operator=(const ProblemDescription& rhs) {
    grids = rhs.grids;
    analysis = rhs.analysis;

    model = rhs.model;

    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        outputRequest->setTarget(
            model.mesh.reassign(outputRequest->getTarget())
        );
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        source->setTarget(
            model.mesh.reassign(source->getTarget())
        );
    }

    return *this;
}
}
