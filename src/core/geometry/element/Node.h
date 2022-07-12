

#pragma once

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class NodeBase : public virtual Base {
public:
    NodeBase() {};
    virtual ~NodeBase() {};

    inline std::size_t numberOfCoordinates() const { return 1; }
    inline std::size_t numberOfFaces   () const { return 1; }
    inline std::size_t numberOfVertices() const { return 1; }
    inline std::size_t numberOfSideVertices   (const std::size_t f = 0) const {
        return 1;
    }
    inline std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 1;
    }
};

template<class T>
class Node : public virtual Element<T>,
             public virtual NodeBase {
public:
    Node();
    Node(const Id id,
         const Coordinate::Coordinate<T,3>* v[1],
         const Layer* lay = nullptr,
         const Model* mat = nullptr);
    Node(const Node<T>& rhs);
    virtual ~Node();
    
    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<Node>(*this);
    }

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>* coord);

    std::unique_ptr<ElemI> toStructured(const CoordI3Group&, const Grid3&,
                        const Math::Real = Grid3::tolerance) const;
    std::unique_ptr<ElemR> toUnstructured(const CoordR3Group&, const Grid3&) const;

private:
    // TODO: Remove plain array
    const Coordinate::Coordinate<T,3>* v_[1];
};

} /* namespace Element */

typedef Element::NodeBase         Nod;
typedef Element::Node<Math::Real> NodR;
typedef Element::Node<Math::Int > NodI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Node.hpp"

