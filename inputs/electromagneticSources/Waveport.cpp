/*
 * Waveport.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "Waveport.h"

Waveport::Waveport(const MagnitudeGaussian* magnitude,
                   const ElementsGroup<Surface<> >& elem,
                   const bool input,
                   const Shape shape,
                   const ExcitationMode excMode,
                   const pair<UInt,UInt> mode)
:   EMSource<>(magnitude),
    ElementsGroup<Surface<> >(elem) {

	input_ = input;
	shape_ = shape;
	excitationMode_ = excMode;
	mode_ = mode;
}

Waveport::Waveport(const Waveport& rhs)
:   EMSource<>(rhs),
    ElementsGroup<Surface<> >(rhs) {

    input_ = rhs.input_;
    shape_ = rhs.shape_;
    excitationMode_ = rhs.excitationMode_;
    mode_ = rhs.mode_;
}

Waveport::~Waveport() {

}

ClassBase* Waveport::clone() const {
    return new Waveport(*this);
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
	EMSource::printInfo();
	if (input_) {
		cout << "- Is input_." << endl;
	} else {
		cout << "- Is output." << endl;
	}
}

string Waveport::getName() const {
    return "Waveport";
}
