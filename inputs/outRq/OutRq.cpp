#include "OutRq.h"

OutRq<void>::OutRq() {
    outputType_ = undefined;
}

OutRq<void>::OutRq(const OutRq<void>::Type outputType,
                   const string& name) {
    name_ = name;
    outputType_ = outputType;
}

OutRq<void>::OutRq(const OutRq& rhs) {
    name_ = rhs.name_;
    outputType_ = rhs.outputType_;
}

OutRq<void>::~OutRq() {

}

const string& OutRq<void>::getName() const {
    return name_;
}

OutRq<void>::Type OutRq<void>::getOutputType() const {
    return outputType_;
}

Domain OutRq<void>::getDomain() const {
    return *this;
}


void OutRq<void>::printInfo() const {
    cout<< "--- Output request instance ---" << endl;
    cout<< "Name: " << name_.c_str() << endl;
    cout<< "Type: " << getTypeStr() << endl;
    this->Domain::printInfo();
    cout<< "--- End of Output Request Instance ---" << endl;
}

string OutRq<void>::getTypeStr() const {
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
