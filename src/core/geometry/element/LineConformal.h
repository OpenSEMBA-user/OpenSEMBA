

#pragma once

#include "geometry/coordinate/Conformal.h"

#include "Line2.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class LineConformal : public virtual Line2<Math::Int> {
public:
    LineConformal(const Id id,
                  const CoordI3* v[2],
                  const Math::CVecR3& norm,
                  const Layer* lay = nullptr,
                  const Model* mat = nullptr);
    LineConformal(const CoordI3* v[2],
                  const Math::CVecR3& norm,
                  const Layer* lay = nullptr,
                  const Model* mat = nullptr);
    LineConformal(const LineConformal& rhs);
    virtual ~LineConformal() = default;

    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<LineConformal>(*this);
    }

    Math::CVecR3 getNorm () const { return norm_;  }

    const CoordConf* getV(const std::size_t i) const;

    void setV(const std::size_t i, const CoordI3* coord);

    std::unique_ptr<ElemI> toStructured(const CoordI3Group&,
        const Grid3&,
        const Math::Real = Grid3::tolerance) const;
    std::unique_ptr<ElemR> toUnstructured(const CoordR3Group&,
        const Grid3&) const;

private:
    void checkCoordinates();
    Math::CVecR3 norm_;
};

namespace Error {
namespace Coord {

class NotConf : public Error {
public:
    NotConf(const CoordId& coordId)
    :   Error(coordId) {
        std::stringstream aux;
        aux << "Coordinate with Id (" << this->getCoordId()
            << ") not conformal";
        str_ = aux.str();
    }
    ~NotConf() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Coord */
} /* namespace Error */
} /* namespace Element */

typedef Element::LineConformal LinConf;

} /* namespace Geometry */
} /* namespace SEMBA */

