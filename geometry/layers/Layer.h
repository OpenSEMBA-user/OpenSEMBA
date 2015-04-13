/*
 * Layer.h
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_LAYER_H_
#define SRC_COMMON_INPUTS_LAYER_H_

#include <string>
#include <iostream>
#include <assert.h>

using namespace std;

#include "../../base/class/ClassIdBase.h"

CREATE_ID(LayerId);

class Layer : public virtual ClassIdBase<LayerId> {
public:
    Layer();
    Layer(const LayerId id, const string& name);
    Layer(const string& name);
    Layer(const Layer& rhs);
    virtual ~Layer();

    DEFINE_CLONE(Layer);

    string getName() const;

    virtual string getParentName() const;
    virtual string getChildName() const;

    virtual void printInfo() const;

private:
    string name_;
};

#endif /* SRC_COMMON_INPUTS_LAYER_H_ */
