#include "Magnitude.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

Magnitude::Magnitude(Math::FunctionRR* mathFunction) {
    mathFunction_ = mathFunction;
}

Magnitude::Magnitude(const Magnitude& rhs) {
    mathFunction_ = dynamic_cast<Math::FunctionRR*>(rhs.mathFunction_->clone());
}

Magnitude& Magnitude::operator=(const Magnitude& rhs) {
    mathFunction_ = dynamic_cast<Math::FunctionRR*>(rhs.mathFunction_->clone());
    return *this;
}

bool Magnitude::operator ==(const Magnitude& rhs) const {
    return *mathFunction_ == *rhs.mathFunction_;
}

Math::Real Magnitude::evaluate(const Math::Real time) const {
    return mathFunction_->operator()(time);
}

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */
