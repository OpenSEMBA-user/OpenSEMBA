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

#include "GroupWithIdBase.h"

template<typename L = Layer>
class LayerGroup : public GroupWithIdBase<L, LayerId> {
public:
    LayerGroup();
    LayerGroup(const vector<L*>&);
    LayerGroup(const GroupBase<L>& rhs);
    virtual ~LayerGroup();

    LayerGroup<L>& operator=(const GroupBase<L>& rhs);

    virtual vector<LayerId> getIds() const;

    virtual void printInfo() const;
private:
    map<uint, const Layer*> layers_;
};

#include "LayerGroup.hpp"

#endif /* SRC_COMMON_INPUTS_LAYERGROUP_H_ */
