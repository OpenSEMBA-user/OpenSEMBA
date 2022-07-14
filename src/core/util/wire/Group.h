#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <type_traits>

#include "ProblemDescription.h"
#include "geometry/element/Polyline.h"
#include "geometry/element/Line2.h"
#include "geometry/graph/Vertices.h"
#include "geometry/mesh/Structured.h"
#include "geometry/mesh/Unstructured.h"
#include "physicalModel/wire/Extremes.h"

namespace SEMBA {
namespace Util {
namespace Wire {

/* Class used to obtain the wires from the lines contained in a SEMBA::Data.
 *
 * It is constructed by a SEMBA::Data. It is templarized in such a way that:
 * - T = Math::Int: the SEMBA::Data contains a structured mesh.
 * - T = Math::Real: it contains a unstructured mesh.
 *
 * The information of the wires is given in the following functions:
 * - numberOfWires(): the number of wires
 * - getWire(i): get the wire i
 * - getMat(matId): get the wire model with id matId
 * - getIdsOfWire(i): get the Ids of the elements that make up the wire i
 * - getRevOfWire(i): for each of the previous elements it informs if that
 *                    element is oriented inversely in the SEMBA::Data
 */
template<class T>
class Group {
public:
    typedef Geometry::Graph::Vertices<
                Geometry::Element::Line<T>,
                Geometry::Coordinate::Coordinate<T,3>> Graph;

    Group(const Model::UnstructuredModel&);
    Group(const Model::StructuredModel&);
    ~Group();

    std::size_t numberOfWires() const { return wires_.size(); }
    const Geometry::Element::Polyline<T>* getWire(const std::size_t& i) const {
        return wires_[i];
    }

    PhysicalModel::Wire::Extremes*       getMat(const MatId& id) {
        return mats_.at(id);
    }
    const PhysicalModel::Wire::Extremes* getMat(const MatId& id) const {
        return mats_.at(id);
    }

    const std::vector<Geometry::ElemId>&
            getIdsOfWire(const std::size_t& i) const {
        return wiresIds_[i];
    }
    const std::vector<bool>& getRevOfWire(const std::size_t& i) const {
        return wiresRev_[i];
    }

private:
    std::vector<Geometry::Element::Polyline<T>*> wires_;
    std::vector<std::vector<Geometry::ElemId>> wiresIds_;
    std::vector<std::vector<bool>>             wiresRev_;

    std::map<MatId, PhysicalModel::Wire::Extremes*> mats_;

    Graph constructGraph_(
        std::vector<const Geometry::Element::Line<T>*>&,
        const PMGroup&
    );
    void fillWiresInfo_(const Graph&, const SEMBA::PMGroup&);
    std::vector<std::vector<const Geometry::Element::Line<T>*>>
        getLines_(const Graph&);
    void getWireMats_(const PhysicalModel::Wire::Wire*& wireMat,
                      const PhysicalModel::Multiport::Multiport*& extremeL,
                      const PhysicalModel::Multiport::Multiport*& extremeR,
                      const std::vector<const Geometry::Element::Line<T>*>&,
                      const SEMBA::PMGroup&,
                      const Graph&);
    Geometry::Element::Polyline<T>* newWire_(
            const std::vector<const Geometry::Element::Line<T>*>& lines,
            PhysicalModel::Wire::Extremes* mat);
};

}
}
}

#include "Group.hpp"

