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

#ifndef SEMBA_GEOMETRY_LAYER_LAYER_H_
#define SEMBA_GEOMETRY_LAYER_LAYER_H_

#include <string>

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

namespace SEMBA {
namespace Geometry {
namespace Layer {

class Layer;
typedef Class::Identification<Layer> Id;

class Layer : public virtual Class::Class,
              public virtual Class::Cloneable,
              public virtual Class::Shareable,
              public virtual Class::Printable,
              public virtual Class::Identifiable<Id> {
public:
    Layer();
    Layer(const Id id, const std::string& name);
    Layer(const std::string& name);
    Layer(const Layer& rhs);
    virtual ~Layer();

    SEMBA_CLASS_DEFINE_CLONE(Layer);

    virtual bool operator==(const Layer& rhs) const;
    virtual bool operator!=(const Layer& rhs) const;

    std::string getName() const;

    virtual std::string getParentName() const;
    virtual std::string getChildName() const;
    std::string toStr() const;

    virtual void printInfo() const;
    friend std::ostream& operator<<(std::ostream& os, const Layer& lay) {
       return os << lay.toStr();
    }

private:
    std::string name_;

    static std::string spaceToUnderscore(std::string rhs);
};

} /* namespace Layer */

typedef Layer::Id LayerId;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_LAYER_LAYER_H_ */
