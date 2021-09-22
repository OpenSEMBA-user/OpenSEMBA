

#include "LineConformal.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

LineConformal::LineConformal() {
    checkCoordinates();
}

LineConformal::LineConformal(const Id id,
                             const CoordI3* v[2],
                             const Math::CVecR3& norm,
                             const Layer* lay,
                             const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat),
    LinI2(v) {

    checkCoordinates();

    norm_  = norm;
}

LineConformal::LineConformal(const CoordI3* v[2],
                             const Math::CVecR3& norm,
                             const Layer* lay,
                             const Model* mat)
:   Elem(lay, mat) {
    checkCoordinates();
    norm_  = norm;
}

LineConformal::LineConformal(const LineConformal& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs),
    LinI2(rhs) {
    norm_  = rhs.norm_;
}

LineConformal::~LineConformal() {

}

const CoordConf* LineConformal::getV(const std::size_t i) const {
	const Coordinate::Coordinate<Math::Int,3>* coord;
	coord = Line2<Math::Int>::getV(i);
    return coord->castTo<CoordConf>();
}

void LineConformal::setV(const std::size_t i, const CoordI3* coord) {
    LinI2::setV(i, coord);
    checkCoordinates();
}

void LineConformal::printInfo() const {
    std::cout << "--- LineConformal info ---" << std::endl;
    std::cout << "Id: " << this->getId() << std::endl;
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        getV(i)->printInfo();
        std::cout << std::endl;
    }
}

void LineConformal::checkCoordinates() {
    for(std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        if (!this->getV(i)->is<CoordConf>()) {
            throw Error::Coord::NotConf(this->getV(i)->getId());
        }
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
