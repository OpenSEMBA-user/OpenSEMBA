#pragma once

#include "math/Types.h"
#include "geometry/Box.h"

#include "class/Class.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

#include "geometry/element/Element.h"
#include "geometry/element/Group.h"

#include "physicalModel/Group.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Mesh;
    typedef Class::Identification<Mesh> Id;

class Mesh : public virtual Class::Identifiable<Id>,
             public virtual Class::Class {
public:
    Mesh() = default;
    virtual ~Mesh() = default;

    virtual void applyScalingFactor(const Math::Real factor) = 0;
    virtual BoxR3 getBoundingBox() const = 0;
    virtual void reassignPointers(const PMGroup& matGr = PMGroup()) = 0;

    virtual std::unique_ptr<Mesh> clone() const = 0;

    virtual ElemView reassign(const ElemView&) = 0;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

