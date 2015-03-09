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

#include "ClassWithIdBase.h"

CreateId(LayerId);

class Layer : public ClassWithIdBase<LayerId> {
public:
    Layer();
    Layer(const LayerId id, const string& name);
    virtual ~Layer();

    virtual ClassBase* clone()                 const;
    virtual ClassBase* clone(const LayerId id) const;

    const string& getName() const;

    virtual string getParentName() const;
    virtual string getChildName() const;

    virtual void printInfo() const;

private:
    string name_;
};

#endif /* SRC_COMMON_INPUTS_LAYER_H_ */
