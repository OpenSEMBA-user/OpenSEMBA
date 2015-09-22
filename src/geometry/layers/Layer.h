// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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

class Layer;

typedef Id<Layer> LayerId;

class Layer : public virtual ClassIdBase<LayerId> {
public:
    Layer();
    Layer(const LayerId id, const string& name);
    Layer(const string& name);
    Layer(const Layer& rhs);
    virtual ~Layer();

    DEFINE_CLONE(Layer);

    virtual bool operator==(const Layer& rhs) const;
    virtual bool operator!=(const Layer& rhs) const;

    string getName() const;

    virtual string getParentName() const;
    virtual string getChildName() const;
    string toStr() const;

    virtual void printInfo() const;
    friend std::ostream& operator<<(ostream& os, const Layer& lay) {
       return os << lay.toStr();
    }

private:
    string name_;
};

#endif /* SRC_COMMON_INPUTS_LAYER_H_ */
