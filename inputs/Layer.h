/*
 * Layer.h
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_LAYER_H_
#define SRC_COMMON_INPUTS_LAYER_H_

#include <string>

using namespace std;

class Layer {
public:
    Layer();
    Layer(uint id, const string& name);
    virtual ~Layer();
    virtual string
     getParentName() const;
    virtual string
     getChildName() const;
    uint
     getId() const;
    const string&
     getName() const;
private:
    uint id_;
    string name_;
};

#endif /* SRC_COMMON_INPUTS_LAYER_H_ */
