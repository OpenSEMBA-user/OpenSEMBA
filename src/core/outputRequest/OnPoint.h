#pragma once

#include "OutputRequest.h"

namespace SEMBA {
namespace OutputRequest {
    class OnPoint : public virtual OutputRequest {
    public:
        OnPoint(
            const Type& outputType,
            const Domain& domain,
            const std::string& name,
            const Target& box
        );
        virtual ~OnPoint() = default;

        std::unique_ptr<OutputRequest> clone() const override {
            return std::make_unique<OnPoint>(*this);
        }
    };

} /* namespace OutputRequest */
} /* namespace SEMBA */
