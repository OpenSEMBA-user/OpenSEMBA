/*
 * Waveport.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "Waveport.h"

Waveport::Waveport(const Magnitude* magnitude,
                   const GroupElements<const Surf>& elem,
                   const ExcitationMode excMode,
                   const pair<UInt,UInt> mode)
:   EMSourceBase(magnitude),
    GroupElements<const Surf>(elem) {

	excitationMode_ = excMode;
	mode_ = mode;
	check();
}

Waveport::Waveport(const Waveport& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Surf>(rhs) {

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
    hasSameProperties &= mode_ == rhsPtr->mode_;
    hasSameProperties &= excitationMode_ == rhsPtr->excitationMode_;
    return hasSameProperties;
}

const string& Waveport::getName() const {
    const static string res = "Waveport";
    return res;
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
    throw ErrorNotImplemented("Waveport::getElectricWeights");
    return toGlobalAxis(res);
}

void Waveport::printInfo() const {
	cout << "--- Waveport info ---" << endl;
	if (getMagnitude() != NULL) {
		cout << "- Is input." << endl;
	} else {
		cout << "- Is output." << endl;
	}

	cout << "Mode: ";
	if (excitationMode_ == TE) {
	    cout << "TE ";
	} else {
	    cout << "TM ";
	}
	cout << mode_.first << ", " << mode_.second << endl;
	EMSource<Surf>::printInfo();
}

CVecR3 Waveport::getNormal() const {
    if (this->getGroupOf<Surf>().size() > 0) {
        if (this->get(0)->is<SurfR>()) {
            return this->get(0)->castTo<SurfR>()->getNormal();
        } else {
            CVecI3 aux = this->get(0)->castTo<SurfI>()->getNormal();
            CVecR3 res;
            for (UInt d = 0; d < 3; d++) {
                res(d) = (Real) aux(d);
            }
            return res;
        }
    }
    return CVecR3();
}

CVecR3 Waveport::getLocalAxis() const {
    CVecR3 localX, localY, localZ;
    localZ = getNormal();
    BoxR3 box = getBound();


}

vector<CVecR3> Waveport::toLocalAxis(const vector<CVecR3>& rhs) const {
}

vector<CVecR3> Waveport::toGlobalAxis(const vector<CVecR3>& rhs) const {
}
