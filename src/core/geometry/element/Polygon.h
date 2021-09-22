

#pragma once

#include "Surface.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Polygon: public Surface<Math::Real> {
public:
    Polygon();
    Polygon(const Id id,
            const std::vector<const CoordR3*>& v,
            const Layer* lay = nullptr,
            const Model* mat = nullptr);
    Polygon(const Polygon& rhs);
    virtual ~Polygon();
    
    SEMBA_CLASS_DEFINE_CLONE(Polygon);
    
    std::size_t numberOfFaces      () const;
    std::size_t numberOfVertices   () const;
    std::size_t numberOfCoordinates() const;

    std::size_t numberOfSideVertices   (const std::size_t f) const;
    std::size_t numberOfSideCoordinates(const std::size_t f) const;

    const CoordR3* getV    (const std::size_t i) const;
    const CoordR3* getSideV(const std::size_t f,
                            const std::size_t i) const;

    const CoordR3* getVertex    (const std::size_t i) const;
    const CoordR3* getSideVertex(const std::size_t f,
                                 const std::size_t i) const;

    Math::Real getArea() const;

    void setV(const std::size_t i, const CoordR3*);
    
    void printInfo() const;

private:
    std::vector<const CoordR3*> v_;
    // TODO void checkVertexCoplanarity() const;

};

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */

