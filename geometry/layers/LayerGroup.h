/*
 * LayerGroup.h
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_LAYERGROUP_H_
#define SRC_COMMON_INPUTS_LAYERGROUP_H_

#include <vector>
#include <map>

using namespace std;

#include "Layer.h"
#include "../../base/group/GroupId.h"

template<typename L = Layer>
class LayerGroup : public virtual GroupId<L, LayerId> {
public:
    USE_GROUP_CONSTRUCTS(LayerGroup, L);

    DEFINE_GROUP_CLONE(LayerGroup, L);

    USE_GROUP_ASSIGN(L);

    USE_GROUPID_GET(L,LayerId);
    const L* get(const string name) const;

    void printInfo() const;
};

#include "LayerGroup.hpp"

#endif /* SRC_COMMON_INPUTS_LAYERGROUP_H_ */
