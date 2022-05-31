#pragma once

#include "OutputRequest.h"

namespace SEMBA {
namespace OutputRequest {
    class OnLine : public virtual OutputRequest {
    public:
        OnLine(
            const Type& outputType,
            const Domain& domain,
            const std::string& name,
            const Target& box
        );
        virtual ~OnLine() = default;

        std::unique_ptr<OutputRequest> clone() const override {
            return std::make_unique<OnLine>(*this);
        }
    };

} /* namespace OutputRequest */
} /* namespace SEMBA */
