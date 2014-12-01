#ifndef OUTPUTREQUEST_H_
#include "OutputRequest.h"
#endif

OutputRequest::OutputRequest(
 const Domain& domain,
 const Element::Type elementType,
 const OutputRequest::Type outputType,
 const string& name,
 const vector<unsigned int>& elements_) : Condition(elementType), Domain(domain) {
	outputType_ = outputType;
	name_ = name;
	usingBound_ = false;
	elem_ = elements_;
	setThetaAndPhi(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

OutputRequest::OutputRequest(
 const Element::Type elementType,
 const OutputRequest::Type outputType,
 const string& name,
 const vector<unsigned int>& elements_) : Condition(elementType) {
	outputType_  = outputType;
	name_ = name;
	usingBound_ = false;
	elem_ = elements_;
	setThetaAndPhi(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

OutputRequest::OutputRequest(
 const Domain& domain,
 const Element::Type elementType,
 const OutputRequest::Type outputType,
 const string& name,
 const BoundingBox& box) : Condition(elementType), Domain(domain) {
	outputType_  = outputType;
	name_ = name;
	usingBound_ = true;
	bound_ = box;
	setThetaAndPhi(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

OutputRequest::OutputRequest(
 const Domain& domain,
 const Element::Type elementType,
 const OutputRequest::Type outputType,
 const string& name,
 const BoundingBox& box,
 const double iTh, const double fTh, const double sTh,
 const double iPhi, const double fPhi, const double sPhi) : Condition(elementType), Domain(domain) {
	assert(outputType == OutputRequest::farField);
	outputType_  = outputType;
	name_ = name;
	usingBound_ = true;
	bound_ = box;
	setThetaAndPhi(iTh, fTh, sTh, iPhi, fPhi, sPhi);
}

string
OutputRequest::outputTypeStr() const {
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
OutputRequest::elementTypeStr() const {
	string res;
	switch (getElementType()) {
	case Element::NODE:
		res = "node";
		break;
	case Element::LINE:
		res = "line";
		break;
	case Element::SURFACE:
		res = "surface";
		break;
	case Element::VOLUME:
		res = "volume";
		break;
	default:
		res = "Unreckognized";
	}
	return res;
}

OutputRequest&
OutputRequest::operator=(const OutputRequest& rhs) {
	if (this == &rhs) {
		return *this;
	}
	Condition::operator=(rhs);
	Domain::operator=(rhs);
	outputType_ = rhs.outputType_;
	usingBound_ = rhs.usingBound_;
	bound_ = rhs.bound_;
	name_ = rhs.name_;
	elem_ = rhs.elem_;
	return *this;
}

void
OutputRequest::printInfo() const {
	cout<< "--- Output request instance ---" << endl;
	cout<< "Name: " << name_.c_str() << endl;
	Domain::printInfo();
	cout<< "Output type: " << outputTypeStr() << " over "<<
		elementTypeStr() << ". " << endl;
	if (usingBound_) {
		cout << "Using bound_: ";
		bound_.printInfo();
	} else {
		cout<< "Number of elements: " << elem_.size() << endl;
	}
	cout<< "--- End of Output Request Instance ---" << endl;
}

bool
OutputRequest::isSimilar(const OutputRequest& rhs) const {
	bool isSimilar = true;
	isSimilar &= name_ == rhs.name_;
	isSimilar &= outputType_ == rhs.outputType_;
	isSimilar &= initialTheta_ == rhs.initialTheta_;
	isSimilar &= finalTheta_ == rhs.finalTheta_;
	isSimilar &= stepTheta_ == rhs.stepTheta_;
	isSimilar &= initialPhi_ == rhs.initialPhi_;
	isSimilar &= finalPhi_ == rhs.finalPhi_;
	isSimilar &= stepPhi_ == rhs.stepPhi_;
	isSimilar &= Domain::operator==(rhs);
	isSimilar &= !usingBound_;
	return isSimilar;
}

void
OutputRequest::setAdditionalElems(const vector<uint> elems) {
	for (uint i = 0; i < elems.size(); i++) {
		elem_.push_back(elems[i]);
	}
}

void
OutputRequest::setThetaAndPhi(
		double iTh, double fTh, double sTh,
		double iPhi, double fPhi, double sPhi) {
	initialTheta_ = iTh;
	finalTheta_ = fTh;
	stepTheta_ = sTh;
	initialPhi_ = iPhi;
	finalPhi_ = fPhi;
	stepPhi_ = sPhi;
}
