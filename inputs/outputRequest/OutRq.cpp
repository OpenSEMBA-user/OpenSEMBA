#include "OutRq.h"

OutRq::OutRq(
 const Domain& domain,
 const Element::Type elementType,
 const OutRq::Type outputType,
 const string& name,
 const vector<ElementId>& elements_) : Condition(elementType), Domain(domain) {
 	name_ = name;
	outputType_ = outputType;
	elem_ = elements_;
}

ClassBase* OutRq::clone() const {
   return new OutRq(*this);
}

string
OutRq::outputTypeStr() const {
	switch (outputType_) {
	case electricField:
		return "Electric field";
	case magneticField:
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
 
string
OutRq::elementTypeStr() const {
	string res;
	switch (getElementType()) {
	case Element::node:
		res = "node";
		break;
	case Element::line:
		res = "line";
		break;
	case Element::surface:
		res = "surface";
		break;
	case Element::volume:
		res = "volume";
		break;
	default:
		res = "Unreckognized";
	}
	return res;
}

OutRq&
OutRq::operator=(const OutRq& rhs) {
	if (this == &rhs) {
		return *this;
	}
	Condition::operator=(rhs);
	Domain::operator=(rhs);
	outputType_ = rhs.outputType_;
	name_ = rhs.name_;
	elem_ = rhs.elem_;
	return *this;
}

const string& OutRq::getName() const {
    return name_;
}

OutRq::Type OutRq::getOutputType() const {
    return outputType_;
}

const vector<ElementId>& OutRq::getElem() const {
    return elem_;
}

void
OutRq::printInfo() const {
	cout<< "--- Output request instance ---" << endl;
	cout<< "Name: " << name_.c_str() << endl;
	Domain::printInfo();
	cout<< "Output type: " << outputTypeStr() << " over "<<
		elementTypeStr() << ". " << endl;
	cout<< "Number of elements: " << elem_.size() << endl;
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

void
OutRq::setAdditionalElems(const vector<ElementId> elems) {
	for (uint i = 0; i < elems.size(); i++) {
		elem_.push_back(elems[i]);
	}
}
