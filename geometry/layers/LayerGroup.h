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
#include "Layer.h"

using namespace std;

#include "../../base/GroupId.h"

template<typename L = Layer>
class LayerGroup : public GroupId<L, LayerId> {
public:
    LayerGroup();
    template<typename L2>
    LayerGroup(const vector<L2*>&);
    template<typename L2>
    LayerGroup(const Group<L2>& rhs);
    virtual ~LayerGroup();

    template<typename L2>
    LayerGroup<L>& operator=(const Group<L2>& rhs);

    const L* get(const string name) const;

    void printInfo() const;
};

#include "LayerGroup.hpp"

#endif /* SRC_COMMON_INPUTS_LAYERGROUP_H_ */
