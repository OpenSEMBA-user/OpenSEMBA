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

class LayerGroup {
public:
    LayerGroup();
    virtual ~LayerGroup();
    virtual void
     add(Layer* layer);
    virtual const Layer*
     get(const uint layerId) const;
    virtual vector<uint>
     getIds() const;
    virtual void
     printInfo() const;
private:
    map<uint, const Layer*> layers_;
};

#endif /* SRC_COMMON_INPUTS_LAYERGROUP_H_ */
