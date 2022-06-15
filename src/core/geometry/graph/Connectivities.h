

#pragma once

#include "geometry/element/Group.h"

#include "Vertices.h"

namespace SEMBA {
namespace Geometry {
namespace Graph {

typedef std::pair<const VolR*, std::size_t> Face;

class Connectivities {
public:
    typedef Element<ElemR,CoordR3> GraphElem;

    Connectivities(const std::vector<const ElemR*>& eG);
    virtual ~Connectivities() = default;

    std::size_t size() const;

    // Returns face of volume matching given face.
    Face getNeighFace(const Face& face) const;

    // Returns surface matching face.
    const SurfR* getNeighSurf(const Face& face) const;

    // Returns face of volume element connecting the surface.
    Face getInnerFace(const SurfR* surf) const;
    Face getOuterFace(const SurfR* surf) const;

    bool isDomainBoundary(const SurfR* surf) const;
    bool isDomainBoundary(Face boundary) const;

    bool existsReciprocity() const;

private:
    Vertices<ElemR,CoordR3> graph_;
    std::map<ElemId, const GraphElem*> index_;

    Face getMatchingFace_(
            const GraphElem* local,
            const std::vector<const CoordR3*> localSideV) const;

};

namespace Error {

class NotReciprocal : public virtual std::exception {
public:
    NotReciprocal(const Face& face)
    :   face_(face) {
        std::stringstream ss;
        ss << "Face of element " << face_.first->getId()
           << " through f " << face_.second
           << " is not reciprocal.";
        str_ = ss.str();
    }
    virtual ~NotReciprocal() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    Face face_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */

