#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

class Gap : public virtual PhysicalModel {
public:
    Gap(const Id id, const std::string name, const Math::Real width);
    Gap(const Gap&);
    virtual ~Gap() = default;

    SEMBA_CLASS_DEFINE_CLONE(Gap);


    Math::Real getWidth() const;

private:
    Math::Real width_;
};

} /* namespace PhysicalModel */
} /* namespace SEMBA */

