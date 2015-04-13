/*
 * Waveport.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "Waveport.h"

Waveport::Waveport(const MagnitudeGaussian* magnitude,
                   const ElementsGroup<Surf>& elem,
                   const bool input,
                   const Shape shape,
                   const ExcitationMode excMode,
                   const pair<UInt,UInt> mode)
:   EMSourceBase(magnitude),
    ElementsGroup<const Surf>(elem) {

	input_ = input;
	shape_ = shape;
	excitationMode_ = excMode;
	mode_ = mode;
}

Waveport::Waveport(const Waveport& rhs)
:   EMSourceBase(rhs),
    ElementsGroup<const Surf>(rhs) {

    input_ = rhs.input_;
    shape_ = rhs.shape_;
    excitationMode_ = rhs.excitationMode_;
    mode_ = rhs.mode_;
}

Waveport::~Waveport() {

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

void Waveport::printInfo() const {
	cout << "--- Waveport info ---" << endl;
	EMSourceBase::printInfo();
	if (input_) {
		cout << "- Is input_." << endl;
	} else {
		cout << "- Is output." << endl;
	}
}
