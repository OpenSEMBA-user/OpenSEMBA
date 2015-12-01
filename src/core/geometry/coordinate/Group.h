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
/*
 * GroupCoordinates.h
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */

#ifndef COMMON_GEOMETRY_COORDINATES_GROUPCOORDINATES_H_
#define COMMON_GEOMETRY_COORDINATES_GROUPCOORDINATES_H_

#include <map>
#include <set>

using namespace std;

#include "Coordinate.h"
#include "base/group/GroupId.h"

struct lexCompareCoord {
    template<class T, Int D>
    bool operator() (const Coordinate<T,D>* lhs,
                     const Coordinate<T,D>* rhs) const {
        return (lhs->pos() < rhs->pos());
    }
};

template<typename C = Coord>
class GroupCoordinates : public GroupId<C, CoordinateId> {
public:
    GroupCoordinates();
    GroupCoordinates(const vector<CVecR3>&);
    GroupCoordinates(const vector<CVecI3>&);
    template<typename C2>
    GroupCoordinates(C2*);
    template<typename C2>
    GroupCoordinates(const std::vector<C2*>&);
    template<typename C2>
    GroupCoordinates(VectorPtr<C2>&);
    template<typename C2>
    GroupCoordinates(const VectorPtr<C2>&);
    GroupCoordinates(VectorPtr<C>&);
    template<typename C2>
    GroupCoordinates(VectorPtr<C2>&&);
    GroupCoordinates(VectorPtr<C>&&);
    virtual ~GroupCoordinates();

    DEFINE_GROUP_CLONE(GroupCoordinates, C);

    GroupCoordinates& operator=(VectorPtr<C>&);
    GroupCoordinates& operator=(VectorPtr<C>&&);

    void clear();

    const CoordR3* getPos(const CVecR3& pos) const;
    const CoordI3* getPos(const CVecI3& pos) const;

    VectorPtr<C> add(VectorPtr<C>&);
    VectorPtr<C> add(VectorPtr<C>&&);
    using GroupId<C,CoordinateId>::add;
    
    const C*            addPos(const CVecR3&        , const bool = false);
    GroupCoordinates<C> addPos(const vector<CVecR3>&, const bool = false);
    const C*            addPos(const CVecI3&        , const bool = false);
    GroupCoordinates<C> addPos(const vector<CVecI3>&, const bool = false);

    void remove(const UInt&);
    void remove(const std::vector<UInt>&);

    void applyScalingFactor(const Real factor);
    
    void printInfo() const;

private:
    multiset<const CoordR3*, lexCompareCoord> indexUnstr_;
    multiset<const CoordI3*, lexCompareCoord> indexStr_;

    void postprocess_(const UInt i);
};

#include "GroupCoordinates.hpp"

typedef GroupCoordinates<CoordR3> CoordR3Group;
typedef GroupCoordinates<CoordI3> CoordI3Group;

#endif /* COMMON_GEOMETRY_COORDINATES_GROUPCOORDINATES_H_ */
