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

#ifndef SEMBA_GEOMETRY_COORDINATE_GROUP_H_
#define SEMBA_GEOMETRY_COORDINATE_GROUP_H_

#include <set>

#include "Coordinate.h"
#include "group/Cloneable.h"
#include "group/Printable.h"
#include "group/Identifiable.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

struct CoordComparator {
    template<class T, Size D>
    bool operator() (const Coordinate<T,D>* lhs,
                     const Coordinate<T,D>* rhs) const {
        return (lhs->pos() < rhs->pos());
    }
};

template<typename C = Coord>
class Group : public SEMBA::Group::Cloneable<C>,
              public SEMBA::Group::Printable<C>,
              public SEMBA::Group::Identifiable<C, Id> {
public:
    Group();
    Group(const std::vector<Math::CVecR3>&);
    Group(const std::vector<Math::CVecI3>&);
    template<typename C2>
    Group(C2*);
    template<typename C2>
    Group(const std::vector<C2*>&);
    template<typename C2>
    Group(SEMBA::Group::Group<C2>&);
    template<typename C2>
    Group(const SEMBA::Group::Group<C2>&);
    Group(SEMBA::Group::Group<C>&);
    template<typename C2>
    Group(SEMBA::Group::Group<C2>&&);
    Group(SEMBA::Group::Group<C>&&);
    virtual ~Group();

    SEMBA_GROUP_DEFINE_CLONE(Group, C);

    Group& operator=(SEMBA::Group::Group<C>&);
    Group& operator=(SEMBA::Group::Group<C>&&);

    void clear();

    const CoordR3* getPos(const Math::CVecR3& pos) const;
    const CoordI3* getPos(const Math::CVecI3& pos) const;

    using SEMBA::Group::Identifiable<C,Id>::add;
    SEMBA::Group::Group<C> add(SEMBA::Group::Group<C>&);
    SEMBA::Group::Group<C> add(SEMBA::Group::Group<C>&&);
    
    const C*               addPos(const Math::CVecR3&,
                                  const bool = false);
    SEMBA::Group::Group<C> addPos(const std::vector<Math::CVecR3>&,
                                  const bool = false);
    const C*               addPos(const Math::CVecI3&,
                                  const bool = false);
    SEMBA::Group::Group<C> addPos(const std::vector<Math::CVecI3>&,
                                  const bool = false);

    void remove(const Size&);
    void remove(const std::vector<Size>&);

    void applyScalingFactor(const Math::Real factor);
    
    void printInfo() const;

private:
    std::multiset<const CoordR3*, CoordComparator> indexUnstr_;
    std::multiset<const CoordI3*, CoordComparator> indexStr_;

    void postprocess_(const Size i);
};

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

namespace SEMBA {
namespace Geometry {

typedef Coordinate::Group<CoordR3> CoordR3Group;
typedef Coordinate::Group<CoordI3> CoordI3Group;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_COORDINATE_GROUP_H_ */
