

#pragma once

#include <vector>

#include "Line.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class PolylineBase : public virtual LineBase {
public:
    PolylineBase() {};
    virtual ~PolylineBase() {};
};

template<class T>
class Polyline : public virtual Line<T>,
                 public virtual PolylineBase {
public:
    Polyline();
    Polyline(const Id id,
             const std::vector<const Coordinate::Coordinate<T,3>*>& v,
             const Layer* lay = nullptr,
             const Model* mat = nullptr);
    Polyline(const Polyline<T>& rhs);
    virtual ~Polyline();
    
    SEMBA_CLASS_DEFINE_CLONE(Polyline);

    inline std::size_t numberOfCoordinates() const { return v_.size(); }

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>* coord);

    void printInfo() const;

private:
    std::vector<const Coordinate::Coordinate<T,3>*> v_;
};

} /* namespace Element */

typedef Element::PolylineBase         Polylin;
typedef Element::Polyline<Math::Real> PolylinR;
typedef Element::Polyline<Math::Int > PolylinI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Polyline.hpp"

