#pragma once

#include "Domain.h"
#include "geometry/element/Group.h"

#include "class/Class.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

namespace SEMBA {
namespace OutputRequest {

class OutputRequest;
    typedef Class::Identification<OutputRequest> Id;

class OutputRequest : public virtual Class::Identifiable<Id>,
                      public virtual Class::Class {
public:
    using Target = Geometry::ElemView;

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
    OutputRequest(const Type& outputType,
                  const std::string& name,
                  const Domain& domain,
                  const Target& elems);

    virtual ~OutputRequest() = default;

    virtual std::unique_ptr<OutputRequest> clone() const = 0;

    std::string getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    Type getType() const { return type_; }
    
    const Domain& domain() const { return domain_;  }
    Domain& domain() { return domain_;  };

    Target getTarget() const { return target_; }
    void setTarget(const Target& target) { target_ = target; }

    std::string getTypeStr() const;
   
private:
    std::string name_;
    Type type_;
	Domain domain_;
    Target target_;
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

