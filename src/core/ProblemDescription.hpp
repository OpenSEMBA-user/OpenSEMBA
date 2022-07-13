#include "ProblemDescription.h"

namespace SEMBA {

template<typename M>
ProblemDescriptionBase<M>::ProblemDescriptionBase(const ProblemDescriptionBase& rhs) {
    grids = rhs.grids;
    analysis = rhs.analysis;
    project = rhs.project;
    
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

template<typename M>
ProblemDescriptionBase<M>& ProblemDescriptionBase<M>::operator=(const ProblemDescriptionBase& rhs) {
    grids = rhs.grids;
    analysis = rhs.analysis;
    project = rhs.project;

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
