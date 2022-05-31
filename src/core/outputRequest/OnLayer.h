#pragma once

#include "OutputRequest.h"

namespace SEMBA {
namespace OutputRequest {
    class OnLayer : public virtual OutputRequest {
    public:
        OnLayer(
            const Type& outputType,
            const Domain& domain,
            const std::string& name,
            const Target& box
        );
        virtual ~OnLayer() = default;

        std::unique_ptr<OutputRequest> clone() const override {
            return std::make_unique<OnLayer>(*this);
        }
    };

} /* namespace OutputRequest */
} /* namespace SEMBA */
