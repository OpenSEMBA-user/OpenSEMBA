

#pragma once

#include "math/function/Function.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

class Magnitude : public virtual Class::Class,
                  public virtual Class::Cloneable,
                  public virtual Class::Shareable {
public:
    Magnitude();
    Magnitude(Math::FunctionRR* mathFunction);
    Magnitude(const Magnitude& rhs);
    virtual ~Magnitude();

    SEMBA_CLASS_DEFINE_CLONE(Magnitude);

    Magnitude& operator=(const Magnitude& rhs);

    virtual bool operator==(const Magnitude&) const;

    Math::Real evaluate(const Math::Real time) const;

private:
    Math::FunctionRR* mathFunction_;
};

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */

