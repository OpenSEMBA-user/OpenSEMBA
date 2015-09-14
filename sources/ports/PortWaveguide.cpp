/*
 * Waveport.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "PortWaveguide.h"

PortWaveguide::PortWaveguide(Magnitude* magnitude,
                   const GroupElements<const Surf>& elem,
                   const ExcitationMode excMode,
                   const pair<UInt,UInt> mode)
:   EMSourceBase(magnitude),
    GroupElements<const Surf>(elem) {

	excitationMode_ = excMode;
	mode_ = mode;
	// Performs checks
	if (!this->getBound().isSurface()) {
	    throw Error("Waveport elements must be contained in a coplanar surface");
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

PortWaveguide::PortWaveguide(const PortWaveguide& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Surf>(rhs) {

    excitationMode_ = rhs.excitationMode_;
    mode_ = rhs.mode_;
}

PortWaveguide::~PortWaveguide() {

}

bool PortWaveguide::hasSameProperties(const EMSourceBase& rhs) const {
    if(!EMSourceBase::hasSameProperties(rhs)) {
        return false;
    }
    const PortWaveguide* rhsPtr = rhs.castTo<PortWaveguide>();
    bool hasSameProperties = true;
    hasSameProperties &= mode_ == rhsPtr->mode_;
    hasSameProperties &= excitationMode_ == rhsPtr->excitationMode_;
    return hasSameProperties;
}

const string& PortWaveguide::getName() const {
    const static string res = "Waveguide port";
    return res;
}

PortWaveguide::ExcitationMode PortWaveguide::getExcitationMode() const {
	return excitationMode_;
}

pair<UInt, UInt> PortWaveguide::getMode() const {
	return mode_;
}

void PortWaveguide::printInfo() const {
	cout << "--- Waveguide port info ---" << endl;
	cout << "Mode: ";
	if (excitationMode_ == TE) {
	    cout << "TE ";
	} else {
	    cout << "TM ";
	}
	cout << mode_.first << ", " << mode_.second << endl;
	Port::printInfo();
}
