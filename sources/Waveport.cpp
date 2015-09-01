/*
 * Waveport.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "../sources/Waveport.h"

Waveport::Waveport(const Magnitude* magnitude,
                   const GroupElements<const Surf>& elem,
                   const ExcitationMode excMode,
                   const pair<UInt,UInt> mode)
:   EMSourceBase(magnitude),
    GroupElements<const Surf>(elem) {

	excitationMode_ = excMode;
	mode_ = mode;
	// Performs checks
	if (!this->getBound().isSurface()) {
	    throw Error("Waveport elements must be contained in a single surface");
	}

	CVecR3 diagonal = this->getBound().getMax() - this->getBound().getMin();
	if (!diagonal.isContainedInPlane(xy)) {
	    throw Error("Waveport must be contained in plane xy.");
	}

	if (this->size() == 0) {
	    throw Error("Waveport must contain some elements.");
	}


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

Waveport::ExcitationMode Waveport::getExcitationMode() const {
	return excitationMode_;
}


pair<UInt, UInt> Waveport::getMode() const {
	return mode_;
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
    if (this->getOf<Surf>().size() > 0) {
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
