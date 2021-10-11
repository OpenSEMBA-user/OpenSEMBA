

#pragma once

#include "math/Constants.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

namespace SEMBA {
namespace PhysicalModel {

class PhysicalModel;
typedef Class::Identification<PhysicalModel> Id;

class PhysicalModel : public virtual Class::Class,
                      public virtual Class::Cloneable,
                      public virtual Class::Shareable,
                      public virtual Class::Identifiable<Id> {
public:
    enum Type {
        PEC,
        PMC,
        SMA,
        classic,
        elecDispersive,
        anisotropic,
        isotropicsibc,
        PML,
        wire,
        gap,
        multiport
    };

    PhysicalModel();
    PhysicalModel(const std::string& name);
    PhysicalModel(const PhysicalModel& rhs);
    virtual ~PhysicalModel();

    const std::string& getName() const;
    void setName(const std::string& newName);

private:
    std::string name_;
};

namespace Error {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

} /* namespace Error */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

