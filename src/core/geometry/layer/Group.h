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
 * GroupLayers.h
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#ifndef COMMON_INPUTS_LAYERGROUP_H_
#define COMMON_INPUTS_LAYERGROUP_H_

#include <vector>
#include <map>

using namespace std;

#include "Layer.h"
#include "base/group/GroupId.h"

template<typename L = Layer>
class GroupLayers : public GroupId<L, LayerId> {
public:
    GroupLayers() {}
    template<typename L2>
    GroupLayers(L2* e)                     : GroupId<L,LayerId>(e) {}
    template<typename L2>
    GroupLayers(const std::vector<L2*>& e) : GroupId<L,LayerId>(e) {}
    template<typename L2>
    GroupLayers(VectorPtr<L2>& rhs)       : GroupId<L,LayerId>(rhs) {}
    template<typename L2>
    GroupLayers(const VectorPtr<L2>& rhs) : GroupId<L,LayerId>(rhs) {}
    GroupLayers(VectorPtr<L>& rhs)        : GroupId<L,LayerId>(rhs) {}
    template<typename L2>
    GroupLayers(VectorPtr<L2>&& rhs) : GroupId<L,LayerId>(std::move(rhs)) {}
    GroupLayers(VectorPtr<L >&& rhs) : GroupId<L,LayerId>(std::move(rhs)) {}
    virtual ~GroupLayers() {}

    DEFINE_GROUP_CLONE(GroupLayers, L);

    GroupLayers& operator=(VectorPtr<L>&);
    GroupLayers& operator=(VectorPtr<L>&&);

    const L* getName(const string name) const;

    void printInfo() const;
};

#include "GroupLayers.hpp"

#endif /* COMMON_INPUTS_LAYERGROUP_H_ */
