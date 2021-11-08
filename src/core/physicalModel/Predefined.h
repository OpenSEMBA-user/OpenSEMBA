#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

template <class T>
class Predefined : public virtual PhysicalModel {
public:   
    class PEC;
    class PMC;
    class SMA;

    Predefined(Id id, const std::string& name) :
        Identifiable<Id>(id), 
        PhysicalModel(name) {
    }
    virtual ~Predefined() = default;
    
    SEMBA_CLASS_DEFINE_CLONE(Predefined<T>);
};

typedef Predefined<Predefined<void>::PEC> PEC;
typedef Predefined<Predefined<void>::PMC> PMC;
typedef Predefined<Predefined<void>::SMA> SMA;

} /* namespace PhysicalModel */
} /* namespace SEMBA */

