#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

class Gap : public virtual PhysicalModel {
public:
    Gap(const Id id, const std::string name, const Math::Real width);
    Gap(const Gap&);
    virtual ~Gap() = default;

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<Gap>(*this);
    }

    Math::Real getWidth() const;

private:
    Math::Real width_;
};

} /* namespace PhysicalModel */
} /* namespace SEMBA */

