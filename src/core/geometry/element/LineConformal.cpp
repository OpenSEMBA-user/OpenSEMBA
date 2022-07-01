

#include "LineConformal.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

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
:   LinI2(ElemId(0), v, lay, mat) {
    checkCoordinates();
    norm_  = norm;
}

LineConformal::LineConformal(const LineConformal& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs),
    LinI2(rhs) {
    norm_  = rhs.norm_;
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

void LineConformal::checkCoordinates() {
    for(std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        if (!this->getV(i)->is<CoordConf>()) {
            throw Error::Coord::NotConf(this->getV(i)->getId());
        }
    }
}

std::unique_ptr<ElemI> LineConformal::toStructured(
    const CoordI3Group& cG,
    const Grid3& grid, const Math::Real tol) const {

    return std::make_unique<LineConformal>(this->getId(),
        this->vertexToStructured(cG, grid, tol).data(),
        norm_,
        this->getLayer(),
        this->getModel());
}

std::unique_ptr<ElemR> LineConformal::toUnstructured(
    const CoordR3Group& cG,
    const Grid3& grid) const {
    throw std::logic_error("LineConformal::toUnstructured operation not permitted");
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
