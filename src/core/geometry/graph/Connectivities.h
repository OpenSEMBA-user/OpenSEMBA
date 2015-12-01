// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#ifndef SEMBA_GEOMETRY_GRAPH_CONNECTIVITIES_H_
#define SEMBA_GEOMETRY_GRAPH_CONNECTIVITIES_H_

#include "geometry/element/Group.h"

#include "Vertices.h"

namespace SEMBA {
namespace Geometry {
namespace Graph {

typedef std::pair<const VolR*, Size> Face;

class Connectivities {
public:
    typedef Element<ElemR,CoordR3> GraphElem;

    Connectivities();
    Connectivities(const Group::Group<const ElemR>& eG);
    virtual ~Connectivities();

    Size size() const;

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

#endif /* SEMBA_GEOMETRY_GRAPH_CONNECTIVITIES_H_ */
