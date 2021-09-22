

#pragma once

#include "Volume.h"
#include "Polygon.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Polyhedron : public Volume<Math::Real> {
public:
    Polyhedron();
    Polyhedron(const Id id,
               const std::vector<const Polygon*>& faces,
               const Layer* lay = nullptr,
               const Model* mat = nullptr);
    Polyhedron(const Polyhedron& rhs);
    virtual ~Polyhedron();

    SEMBA_CLASS_DEFINE_CLONE(Polyhedron);

    bool isCurvedFace(const std::size_t f) const;

    std::size_t numberOfFaces      () const;
    std::size_t numberOfVertices   () const;
    std::size_t numberOfCoordinates() const;

    std::size_t numberOfSideVertices   (const std::size_t f = 0) const;
    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const;
    const CoordR3* getV(const std::size_t i) const;
    const CoordR3* getSideV(const std::size_t f, const std::size_t i) const;
    const CoordR3* getVertex(const std::size_t i) const;
    const CoordR3* getSideVertex(const std::size_t f,
                                 const std::size_t i) const;

    const Polygon* getFace(const std::size_t f) const;
    Math::Real getAreaOfFace(const std::size_t f) const;
    Math::Real getVolume() const;

    void printInfo() const;

private:
    std::vector<const CoordR3*> v_;
    std::vector<const Polygon*> face_;

    void addV(const CoordR3*);
    void checkClosedness() const;
};

namespace Error {

class NotClosed : public Error {
public:
    NotClosed() {}
    ~NotClosed() throw () {}

    const char* what() const throw() { return "Polyhedron is not closed"; }
};

} /* namespace Error */
} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */

