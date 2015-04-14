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
#include <sstream>
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

    bool operator==(const Layer& rhs) const;
    bool operator!=(const Layer& rhs) const;
    string toStr() const;

    virtual void printInfo() const;

private:
    string name_;
};

template <class T, Int D>
std::ostream& operator<<(ostream& os, const Layer& lay) {
   return os << lay.toStr();
}

#endif /* SRC_COMMON_INPUTS_LAYER_H_ */
