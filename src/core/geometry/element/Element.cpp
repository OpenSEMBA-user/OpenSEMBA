

#include "Element.h"

#include <algorithm>

namespace SEMBA {
namespace Geometry {
namespace Element {

Base::Base(const Layer* lay,
           const Model* mat) {
    lay_ = lay;
    mat_ = mat;
}

Base::Base(const Base& rhs) {
    lay_ = rhs.lay_;
    mat_ = rhs.mat_;
}

Base::~Base() {

}

bool Base::operator==(const Base& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return true;
    }
    return false;
}

LayerId Base::getLayerId() const {
    if (lay_ == nullptr) {
        return LayerId(0);
    }
    return lay_->getId();
}

MatId Base::getMatId  () const {
    if (mat_ == nullptr) {
        return MatId(0);
    }
    return mat_->getId();
}

bool Base::operator!=(const Base& rhs) const {
    return !(*this == rhs);
}

std::vector<CoordId> Base::ascendingIdOrder(
        const std::vector<CoordId>& in) {
    std::vector<CoordId> res = in;
    std::sort(res.begin(), res.end());
    return res;
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
