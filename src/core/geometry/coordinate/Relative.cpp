#include "Relative.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

Relative::Relative(const Id id,
                   const Math::CVecR3& rel)
:   Identifiable<Id>(id) {
    for (std::size_t d = 0; d < 3; d++) {
        this->pos()(d) = (int)std::floor(rel(d));
        this->rel_ (d) = rel(d) - this->pos()(d);
    }
}

Relative::Relative(const Id id,
                   const Math::CVecI3& pos,
                   const Math::CVecR3& rel)
:   Identifiable<Id>(id),
    Math::CVecI3(pos) {
    rel_ = rel;
}

Relative::Relative(const Math::CVecR3& rel) {
    for (std::size_t d = 0; d < 3; d++) {
        this->pos()(d) = (int)std::floor(rel(d));
        this->rel_(d) = rel(d) - this->pos()(d);
    }
}

Relative::Relative(const Relative& rhs)
:   Identifiable<Id>(rhs),
    Math::CVecI3(rhs) {
    rel_ = rhs.rel_;
}

Relative& Relative::operator=(const Relative& rhs) {
    if (this == &rhs)
        return *this;

    CoordI3::operator=(rhs);
    rel_ = rhs.rel_;
    return *this;
}

bool Relative::operator==(const Base& rhs) const {
    if (!Coordinate<Math::Int,3>::operator==(rhs)) {
        return false;
    }
    const Relative* rhsPtr = rhs.castTo<Relative>();
    bool res = true;
    res &= (this->rel_ == rhsPtr->rel_);
    return res;
}

CoordR3* Relative::toUnstructured(const Grid3& grid) const {
    Math::CVecR3 pos = grid.getPos(*this);
    for (std::size_t d = 0; d < 3; d++) {
        Math::Real length = rel_(d);
        Math::Int cellDir;
        if (this->pos()(d) == grid.getNumCells()(d)) {
            cellDir = this->pos()(d);
        }
        else {
            cellDir = this->pos()(d) + 1;
        }
        Math::Real posAux = grid.getPos(d, cellDir);
        Math::Real step = posAux - pos(d);
        pos(d) += step * length;
    }
    return new CoordR3(this->getId(), pos);
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
