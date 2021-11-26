#pragma once

#include "math/Types.h"
#include "geometry/Box.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"

#include "geometry/element/Element.h"
#include "geometry/element/Group.h"

#include "physicalModel/Group.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Mesh : public virtual Class::Class,
             public virtual Class::Cloneable {
public:
    Mesh() = default;
    virtual ~Mesh() = default;

    virtual void applyScalingFactor(const Math::Real factor) = 0;
    virtual BoxR3 getBoundingBox() const = 0;
    virtual void reassignPointers(const PMGroup& matGr = PMGroup()) = 0;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

