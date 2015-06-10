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
class GroupLayers : public virtual GroupId<L, LayerId> {
public:
    USE_GROUP_CONSTRUCTS(GroupLayers, L);

    DEFINE_GROUP_CLONE(GroupLayers, L);

    USE_GROUP_ASSIGN(L);

    USE_GROUPID_GET(L,LayerId);
    const L* get(const string name) const;

    void printInfo() const;
};

#include "GroupLayers.hpp"

#endif /* COMMON_INPUTS_LAYERGROUP_H_ */
