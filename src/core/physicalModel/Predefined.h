#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

class PredefinedGeneric : public virtual PhysicalModel {
};

template <class T>
class Predefined : public PredefinedGeneric {
public:   
    class PEC;
    class PMC;
    class SMA;

    Predefined(Id id, const std::string& name) :
        Identifiable<Id>(id), 
        PhysicalModel(name) {
    }
    virtual ~Predefined() = default;
    
    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<Predefined<T>>(*this);
    }
};

typedef Predefined<Predefined<void>::PEC> PEC;
typedef Predefined<Predefined<void>::PMC> PMC;
typedef Predefined<Predefined<void>::SMA> SMA;

} /* namespace PhysicalModel */
} /* namespace SEMBA */

