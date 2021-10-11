

#include "Conformal.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

Conformal::Conformal() {
    dir_ = Math::Constants::CartesianAxis::x;
    length_ = 0.0;
}

Conformal::Conformal(const Id id,
                     const Math::CVecI3& pos,
                     const Math::Constants::CartesianAxis dir,
                     const Math::Real length)
:   Identifiable<Id>(id),
    Math::CVecI3(pos) {

    dir_    = dir;
    length_ = length;
}

Conformal::Conformal(const Math::Constants::CartesianAxis dir,
                     const Math::Real length) {

    dir_    = dir;
    length_ = length;
}

Conformal::Conformal(const Conformal& rhs)
:   Identifiable<Id>(rhs),
    Math::CVecI3(rhs) {

    dir_    = rhs.dir_;
    length_ = rhs.length_;
}

Conformal::~Conformal() {

}

Conformal& Conformal::operator=(const Conformal& rhs) {
    if (this == &rhs)
        return *this;

    CoordI3::operator=(rhs);
    dir_    = rhs.dir_;
    length_ = rhs.length_;

    return *this;
}

bool Conformal::operator==(const Base& rhs) const {
    if (!Coordinate<Math::Int,3>::operator==(rhs)) {
        return false;
    }
    const Conformal* rhsPtr = rhs.castTo<Conformal>();
    bool res = true;
    res &= (this->length_ == rhsPtr->length_);
    res &= (this->dir_ == rhsPtr->dir_);
    return res;
}

CoordR3* Conformal::toUnstructured(const Grid3& grid) const {
    Math::CVecR3 pos = grid.getPos(*this);
    if (Math::Util::greater(getLength(), 0.0)) {
        Math::Int dir = getDir();
        Math::Real length = getLength();
        Math::CVecI3 cellAux = *this;
        cellAux(dir)++;
        Math::CVecR3 posAux = grid.getPos(cellAux);
        Math::Real step = posAux(dir)-pos(dir);
        pos(dir) += step*length;
    }
    return new CoordR3(this->getId(), pos);
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
