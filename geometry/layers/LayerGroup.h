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

#include "GroupId.h"

template<typename L = Layer>
class LayerGroup : public GroupId<L, LayerId> {
public:
    LayerGroup();
    LayerGroup(const vector<L*>&);
    LayerGroup(const Group<L>& rhs);
    virtual ~LayerGroup();

    LayerGroup<L>& operator=(const Group<L>& rhs);

    virtual vector<LayerId> getIds() const;

    virtual void printInfo() const;
private:
    map<uint, const Layer*> layers_;
};

#include "LayerGroup.hpp"

#endif /* SRC_COMMON_INPUTS_LAYERGROUP_H_ */
