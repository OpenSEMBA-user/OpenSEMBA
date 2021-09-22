

#pragma once

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

