

#pragma once

#include "Domain.h"
#include "geometry/element/Group.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"

namespace SEMBA {
namespace OutputRequest {

class OutputRequest : public virtual Class::Class,
                      public virtual Class::Cloneable,
                      public virtual Class::Shareable {
public:
    typedef typename std::vector<const Geometry::Elem*> TargetElements;

    enum class Type {
        electric,
        magnetic,
        electricFieldNormals,
        magneticFieldNormals,
		surfaceCurrentDensity,
        bulkCurrentElectric,
        bulkCurrentMagnetic,
        current,
		voltage,
        electricFarField
    };

    OutputRequest() = default;
    OutputRequest(const Domain& domain,
                  const Type outputType,
                  const std::string& name,
                  const TargetElements& elems);

    OutputRequest(const OutputRequest& rhs) = default;
    virtual ~OutputRequest() = default;

    SEMBA_CLASS_DEFINE_CLONE(OutputRequest);

    std::string getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    Type getType() const { return type_; }
    
    const Domain& domain() const { return domain_;  }
    Domain& domain() { return domain_;  };

    TargetElements getTarget() const { return target_; }
    void setTarget(const TargetElements& target) { target_ = target; }

    std::string getTypeStr() const;
   
private:
    std::string name_;
    Type type_;
	Domain domain_;
    TargetElements target_;
};

namespace Error {
class Material {
public:
    Material() {}
    virtual ~Material() throw() {}

    const char* what() const throw() {
        return ("Bulk currents have to be defined over elements with no material assigned.");
    }
};

} /* namespace Error */
} /* namespace OutputRequest */
} /* namespace SEMBA */

#include "OutputRequest.hpp"

namespace SEMBA {

} /* namespace SEMBA */

