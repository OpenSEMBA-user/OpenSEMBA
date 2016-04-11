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

#ifndef SEMBA_GEOMETRY_ELEMENT_GROUP_H_
#define SEMBA_GEOMETRY_ELEMENT_GROUP_H_

#include "Element.h"
#include "Node.h"
#include "Line.h"
#include "Surface.h"
#include "Volume.h"

#include "group/Cloneable.h"
#include "group/Printable.h"
#include "group/Identifiable.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

typedef std::pair<const VolR*, std::size_t> Face;

struct CompVecIds {
    bool operator()(const std::vector<CoordId>& lhs,
                    const std::vector<CoordId>& rhs) const {
        std::vector<CoordId> lhsOrdered, rhsOrdered;
        lhsOrdered = Base::ascendingIdOrder(lhs);
        rhsOrdered = Base::ascendingIdOrder(rhs);
        if (lhsOrdered.size() < rhsOrdered.size()) {
            return true;
        } else if (lhsOrdered.size() > rhsOrdered.size()) {
            return false;
        } else {
            for (std::size_t i = 0; i < lhsOrdered.size(); i++) {
                if (lhsOrdered[i] < rhsOrdered[i]) {
                    return true;
                }
                if (lhsOrdered[i] > rhsOrdered[i]) {
                    return false;
                }
            }
            return false;
        }
    }
};

typedef std::map<std::vector<CoordId>,const Elem*,CompVecIds> IndexByVertexId;

template<typename E = Elem>
class Group : public SEMBA::Group::Cloneable<E>,
              public SEMBA::Group::Printable<E>,
              public SEMBA::Group::Identifiable<E, Id> {
public:
    Group() {}
    template<typename E2>
    Group(E2* e)                     : SEMBA::Group::Identifiable<E,Id>(e) {}
    template<typename E2>
    Group(const std::vector<E2*>& e) : SEMBA::Group::Identifiable<E,Id>(e) {}
    template<typename E2>
    Group(SEMBA::Group::Group<E2>&       rhs)
    :   SEMBA::Group::Identifiable<E,Id>(rhs) {}
    template<typename E2>
    Group(const SEMBA::Group::Group<E2>& rhs)
    :   SEMBA::Group::Identifiable<E,Id>(rhs) {}
    Group(SEMBA::Group::Group<E>&        rhs)
    :   SEMBA::Group::Identifiable<E,Id>(rhs) {}
    template<typename E2>
    Group(SEMBA::Group::Group<E2>&& rhs)
    :   SEMBA::Group::Identifiable<E,Id>(std::move(rhs)) {}
    Group(SEMBA::Group::Group<E >&& rhs)
    :   SEMBA::Group::Identifiable<E,Id>(std::move(rhs)) {}
    virtual ~Group() {}

    SEMBA_GROUP_DEFINE_CLONE(Group, E);

    Group& operator=(SEMBA::Group::Group<E>&);
    Group& operator=(SEMBA::Group::Group<E>&&);
    Group& operator=(const SEMBA::Group::Group<E>&);

    bool isLinear() const;

    Group<E>       getMatId(const MatId matId);
    Group<E>       getMatId(const std::vector<MatId>& matId);
    Group<const E> getMatId(const MatId matId) const;
    Group<const E> getMatId(const std::vector<MatId>& matId) const;

    Group<E>       getLayerId(const LayerId layerId);
    Group<E>       getLayerId(const std::vector<LayerId>& layerId);
    Group<const E> getLayerId(const LayerId layerId) const;
    Group<const E> getLayerId(const std::vector<LayerId>& layerId) const;

    Group<E>       getMatLayerId(const MatId, const LayerId);
    Group<const E> getMatLayerId(const MatId, const LayerId) const;

    std::vector<Id> getIdsWithMaterialId   (const MatId matId) const;
    std::vector<Id> getIdsWithoutMaterialId(const MatId matId) const;
    Group<const ElemR> getInsideBound(const BoxR3& bound) const;

    std::vector<std::pair<const E*,std::size_t>> getElementsWithVertex(
            const CoordId) const;
    BoxR3 getBound() const;
    BoxR3 getBound(const std::vector<Face>& border) const;
    virtual const CoordR3* getClosestVertex(const Math::CVecR3 pos) const;

    void setModel(const Model* newMat);
    void setLayer(const Layer* newLay);
    void setModel(const Id id, const Model* newMat);
    void setLayer(const Id id, const Layer* newLay);

    std::map<LayerId, std::vector<const E*> > separateByLayers() const;
    IndexByVertexId getIndexByVertexId() const;

    void removeMatId(const MatId matId);
    void removeMatId(const std::vector<MatId>& matId);

    void linearize();

    void printInfo() const;

protected:
    template<class T>
    void reassignPointers(
            const Coordinate::Group<Coordinate::Coordinate<T,3>>& vNew);
    void reassignPointers(const SEMBA::Geometry::Layer::Group<Layer>& lNew);
    void reassignPointers(const SEMBA::Group::Identifiable<Model,MatId>& mNew);
private:
    std::vector<std::size_t> getElemsWith_(const std::vector<MatId>&) const;
    std::vector<std::size_t> getElemsWith_(const std::vector<LayerId>&) const;
    std::vector<std::size_t> getElemsWith_(const MatId&, const LayerId&) const;
};

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

namespace SEMBA {
namespace Geometry {

typedef Element::Group<      ElemR>      ElemRGroup;
typedef Element::Group<const ElemR> ConstElemRGroup;

typedef Element::Group<      SurfR>      SurfRGroup;

} /* namespace Geometry */
} /* namespace SEMBA */


#endif /* SEMBA_GEOMETRY_ELEMENT_GROUP_H_ */
