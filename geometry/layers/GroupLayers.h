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
