#include "OutRq.h"

OutRq::OutRq(
 const Domain& domain,
 const OutRq::Type outputType,
 const string& name,
 const ElementsGroup<>& elements)
: Domain(domain), ElementsGroup<>(elements) {
 	name_ = name;
	outputType_ = outputType;
}

ClassBase* OutRq::clone() const {
   return new OutRq(*this);
}

string
OutRq::outputTypeStr() const {
	switch (outputType_) {
	case electric:
		return "Electric field";
	case magnetic:
		return "Magnetic field";
	case electricFieldNormals:
		return "Electric field normals";
	case magneticFieldNormals:
		return "Magnetic field normals";
	case bulkCurrentElectric:
		return "Electric bulk current";
	case bulkCurrentMagnetic:
		return "Magnetic bulk current";
	case powerDensity:
		return "Power density";
	case power:
		return "Power";
	case current:
		return "Current";
	case voltage:
		return "Voltage";
	case farField:
		return "Electric Far field";
	default:
		return "Undefined";
	}
}

OutRq&
OutRq::operator=(const OutRq& rhs) {
	if (this == &rhs) {
		return *this;
	}
	Condition::operator=(rhs);
	Domain::operator=(rhs);
	ElementsGroup<>::operator=(rhs);
	outputType_ = rhs.outputType_;
	name_ = rhs.name_;
	return *this;
}

const string& OutRq::getName() const {
    return name_;
}

OutRq::Type OutRq::getOutputType() const {
    return outputType_;
}

void
OutRq::printInfo() const {
	cout<< "--- Output request instance ---" << endl;
	cout<< "Name: " << name_.c_str() << endl;
	Domain::printInfo();
	cout<< "Number of elements: " << size() << endl;
	cout<< "--- End of Output Request Instance ---" << endl;
}

bool
OutRq::isSimilar(const OutRq& rhs) const {
	bool isSimilar = true;
	isSimilar &= name_ == rhs.name_;
	isSimilar &= outputType_ == rhs.outputType_;
	isSimilar &= Domain::operator==(rhs);
	return isSimilar;
}
