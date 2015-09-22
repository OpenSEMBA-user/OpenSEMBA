#include "OutRq.h"

OutRqBase::OutRqBase() {
    outputType_ = undefined;
}

OutRqBase::OutRqBase(const Type outputType,
                     const string& name) {
    name_ = name;
    outputType_ = outputType;
}

OutRqBase::OutRqBase(const OutRqBase& rhs) {
    name_ = rhs.name_;
    outputType_ = rhs.outputType_;
}

OutRqBase::~OutRqBase() {

}

bool OutRqBase::hasSameProperties(const OutRqBase& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    bool hasSameProperties = true;
    hasSameProperties &= getName() == rhs.getName();
    hasSameProperties &= getOutputType() == rhs.getOutputType();
    hasSameProperties &= Domain::operator==(rhs);
    return hasSameProperties;
}

const string& OutRqBase::getName() const {
    return name_;
}

OutRqBase::Type OutRqBase::getOutputType() const {
    return outputType_;
}

Domain OutRqBase::getDomain() const {
    return *this;
}


void OutRqBase::printInfo() const {
    cout<< "Name: " << name_.c_str() << endl;
    cout<< "Type: " << getTypeStr() << endl;
    Domain::printInfo();
}

string OutRqBase::getTypeStr() const {
    switch (outputType_) {
    case electric:
        return "Electric field";
    case magnetic:
        return "Magnetic field";
    case electricFieldNormals:
        return "Electric field normals";
    case magneticFieldNormals:
        return "Magnetic field normals";
    case powerDensity:
        return "Power density";
    case power:
        return "Power";
    case current:
        return "Current";
    case voltage:
        return "Voltage";
    default:
        return "Undefined";
    }
}
