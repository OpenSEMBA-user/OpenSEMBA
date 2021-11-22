#include "Coordinate.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

bool Base::operator ==(const Base& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return true;
    }
    return false;
}

bool Base::operator !=(const Base& rhs) const {
    return !(*this == rhs);
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
