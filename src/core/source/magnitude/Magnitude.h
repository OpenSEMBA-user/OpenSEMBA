#pragma once

#include "math/function/Function.h"

#include "class/Class.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

class Magnitude : public virtual Class::Class {
public:
    Magnitude() = default;
    Magnitude(Math::FunctionRR* mathFunction);
    Magnitude(const Magnitude& rhs);
    virtual ~Magnitude() = default;

    Magnitude& operator=(const Magnitude& rhs);

    virtual std::unique_ptr<Magnitude> clone() const {
        return std::make_unique<Magnitude>(*this);
    }

    virtual bool operator==(const Magnitude&) const;

    Math::Real evaluate(const Math::Real time) const;

private:
    Math::FunctionRR* mathFunction_;
};

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */

