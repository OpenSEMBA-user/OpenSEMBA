

#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Gap {

class Gap : public virtual PhysicalModel {
public:
    Gap(const Id id, const std::string name, const Math::Real width);
    Gap(const Gap&);
    virtual ~Gap();

    SEMBA_CLASS_DEFINE_CLONE(Gap);

    void printInfo() const;
    Math::Real getWidth() const;

private:
    Math::Real width_;
};

} /* namespace Gap */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

