

#include "OutputRequest.h"

#include <iostream>

namespace SEMBA {
namespace OutputRequest {

Base::Base() {

}

Base::Base(const Type outputType,
           const std::string& name,
		   const Domain& domain) {
    name_ = name;
    type_ = outputType;
	domain_ = domain;
}

Base::Base(const Base& rhs) {
    name_ = rhs.name_;
    type_ = rhs.type_;
	domain_ = rhs.domain_;
}

Base::~Base() {

}

const std::string& Base::getName() const {
    return name_;
}

void Base::setName(std::string name) {
    name_ = name;
}


Base::Type Base::getType() const {
    return type_;
}

const Domain& Base::domain() const {
    return domain_;
}

Domain& Base::domain() {
	return domain_;
}

std::string Base::getTypeStr() const {
    switch (type_) {
    case electric:
        return "Electric field";
    case magnetic:
        return "Magnetic field";
	case bulkCurrentElectric:
		return "Bulk current electric";
	case bulkCurrentMagnetic:
		return "Bulk current magnetic";
	case surfaceCurrentDensity:
		return "Surface current density";
    case electricFieldNormals:
        return "Electric field normals";
    case magneticFieldNormals:
        return "Magnetic field normals";
    case current:
        return "Current";
    case voltage:
        return "Voltage";
	case electricFarField:
		return "Electric far field";
    default:
        throw std::logic_error("Unrecognized output type");
    }
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
