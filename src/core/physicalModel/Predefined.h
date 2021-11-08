#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

class Predefined : public virtual PhysicalModel {
public:
    enum class Type {
        pec,
        pmc,
        sma
    };

    Predefined(Id id, const std::string& name, Type inType) :
        Identifiable<Id>(id), 
        PhysicalModel(name), 
        type(inType) {
    }
    virtual ~Predefined() = default;
    
    SEMBA_CLASS_DEFINE_CLONE(Predefined);

    Type getType() const 
    {
        return type;
    }

private:
    Type type;
};

} /* namespace PhysicalModel */
} /* namespace SEMBA */

