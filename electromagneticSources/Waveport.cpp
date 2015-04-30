/*
 * Waveport.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "Waveport.h"

Waveport::Waveport(const Magnitude* magnitude,
                   const GroupElements<const Surf>& elem,
                   const Shape shape,
                   const ExcitationMode excMode,
                   const pair<UInt,UInt> mode)
:   EMSourceBase(magnitude),
    GroupElements<const Surf>(elem) {

	shape_ = shape;
	excitationMode_ = excMode;
	mode_ = mode;
	// Checks surface normal points to same place.
	for (UInt i = 0; i < this->size(); i++) {
	    if (getNormal() != this->get(i)->getNormal()) {
	        cerr << endl << "ERROR @ Waveport ctor: " <<
	                "Surface elements must have the same normal." << endl;
	    }
	}
}

Waveport::Waveport(const Waveport& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Surf>(rhs) {

    shape_ = rhs.shape_;
    excitationMode_ = rhs.excitationMode_;
    mode_ = rhs.mode_;
}

Waveport::~Waveport() {

}

bool Waveport::hasSameProperties(const EMSourceBase& rhs) const {
    if(!EMSourceBase::hasSameProperties(rhs)) {
        return false;
    }
    const Waveport* rhsPtr = rhs.castTo<Waveport>();
    bool hasSameProperties = true;
    hasSameProperties &= shape_ == rhsPtr->shape_;
    hasSameProperties &= mode_ == rhsPtr->mode_;
    hasSameProperties &= excitationMode_ == rhsPtr->excitationMode_;
    return hasSameProperties;
}

const string& Waveport::getName() const {
    const static string res = "Waveport";
    return res;
}

Waveport::Shape
Waveport::getShape() const {
	return shape_;
}

Waveport::ExcitationMode
Waveport::getExcitationMode() const {
	return excitationMode_;
}


pair<UInt, UInt>
Waveport::getMode() const {
	return mode_;
}

vector<CVecR3> Waveport::getElectricWeights(
        const vector<CVecR3>& pos,
        const BoundTerminations& termination) const {
    vector<CVecR3> res(pos.size());
    pos = toLocalAxis(pos);
    switch (getShape()) {
    case Waveport::rectangular:
        const Real m = M_PI * getMode().first / getWidth(termination);
        const Real n = M_PI * getMode().second / getHeight(termination);
        if (getExcitationMode() == Waveport::TE) {
            for (uint i = 0; i < pos.size(); i++) {
                res[i](x) = n * cos(m * pos[i](x)) * sin(n * pos[i](y));
                res[i](y) = - m * sin(m * pos[i](x)) * cos(n * pos[i](y));
                res[i](z) = (Real) 0.0;
            }
        } else {
            for (uint i = 0; i < pos.size(); i++) {
                res[i](x) = - m * cos(m * pos[i](x)) * sin(n * pos[i](y));
                res[i](y) = - m * sin(m * pos[i](x)) * cos(n * pos[i](y));
                res[i](z) = (Real) 0.0;
            }
        }
        break;
    default:
        cerr << endl << "ERROR @ Waveport: Undefined Waveport." << endl;
    }
    return toGlobalAxis(res);
}

void Waveport::printInfo() const {
	cout << "--- Waveport info ---" << endl;
	if (getMagnitude() != NULL) {
		cout << "- Is input." << endl;
	} else {
		cout << "- Is output." << endl;
	}
	EMSource<Surf>::printInfo();
}

CVecR3 Waveport::getNormal() const {
    CVecR3 res;
    if (this->size() > 0) {
        return this->get(0)->getNormal();
    } else {
        cerr << endl << "ERROR @ Waveport: Does not contain surfaces." << endl;
        printInfo();
    }
    return res;
}

CVecR3 Waveport::getLocalAxis() const {
    CVecR3 localZ = getNormal();

    CVecR3 localX = getWidthDir();
}

CVecR3 Waveport::getOffset() const {
}

Real Waveport::getWidth(const BoundTerminations& symmetries) const {
}

Real Waveport::getHeight(const BoundTerminations& symmetries) const {
}

vector<CVecR3> Waveport::toLocalAxis(const vector<CVecR3>& rhs) const {
}

vector<CVecR3> Waveport::toGlobalAxis(const vector<CVecR3>& rhs) const {
}
