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

#ifndef SEMBA_UTIL_WIRE_GROUP_H_
#define SEMBA_UTIL_WIRE_GROUP_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <type_traits>

#include "Data.h"
#include "geometry/element/Polyline.h"
#include "geometry/graph/Vertices.h"
#include "geometry/mesh/Structured.h"
#include "geometry/mesh/Unstructured.h"
#include "physicalModel/wire/Extremes.h"

namespace SEMBA {
namespace Util {
namespace Wire {

template<class T>
class Group {
public:
    typedef Geometry::Graph::Vertices<
                Geometry::Element::Line<T>,
                Geometry::Coordinate::Coordinate<T,3>> Graph;

    Group(const Data&);
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

    void printInfo() const;

private:
    std::vector<Geometry::Element::Polyline<T>*> wires_;
    std::vector<std::vector<Geometry::ElemId>> wiresIds_;
    std::vector<std::vector<bool>>             wiresRev_;

    std::map<MatId, PhysicalModel::Wire::Extremes*> mats_;

    void init_(const Data&);
    Graph constructGraph_(const Data&);
    void fillWiresInfo_(const Graph&,
                        const Data&);
    std::vector<std::vector<const Geometry::Element::Line<T>*>>
        getLines_(const Graph&);
    void getWireMats_(const PhysicalModel::Wire::Wire*& wireMat,
                      const PhysicalModel::Multiport::Multiport*& extremeL,
                      const PhysicalModel::Multiport::Multiport*& extremeR,
                      const std::vector<const Geometry::Element::Line<T>*>&,
                      const Data&);
    Geometry::Element::Polyline<T>* newWire_(
            const std::vector<const Geometry::Element::Line<T>*>& lines,
            const PhysicalModel::PhysicalModel* mat);
};

}
}
}

#include "Group.hpp"

#endif
