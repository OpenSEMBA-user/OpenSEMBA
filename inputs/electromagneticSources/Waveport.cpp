/*
 * Waveport.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "Waveport.h"

Waveport::Waveport(
 const vector<unsigned int>& elem_,
 const bool input_,
 const double spread_,
 const double delay_,
 const Shape shape_,
 const ExcitationMode excMode_,
 const pair<unsigned int,unsigned int> mode_,
 const Symmetry xy_,
 const Symmetry yz_,
 const Symmetry zx_) {
	elem = elem_;
	input = input_;
	spread = spread_;
	delay = delay_;
	shape = shape_;
	excitationMode = excMode_;
	mode = mode_;
	SymXY = xy_;
	SymYZ = yz_;
	SymZX = zx_;
}

Waveport::~Waveport() {

}

Waveport&
Waveport::operator=(const Waveport& rhs) {
	if (this == &rhs) {
		return *this;
	}
	elem = rhs.elem;
	input = rhs.input;
	spread = rhs.spread;
	delay = rhs.delay;
	shape = rhs.shape;
	excitationMode = rhs.excitationMode;
	mode = rhs.mode;
	SymXY = rhs.SymXY;
	SymYZ = rhs.SymYZ;
	SymZX = rhs.SymZX;
	return *this;
}

Waveport::Shape
Waveport::getShape() const {
	return shape;
}

Waveport::Symmetry
Waveport::getSymXY() const {
	return SymXY;
}

Waveport::Symmetry
Waveport::getSymYZ() const {
	return SymYZ;
}

Waveport::ExcitationMode
Waveport::getExcitationMode() const {
	return excitationMode;
}


pair<unsigned int, unsigned int>
Waveport::getMode() const {
	return mode;
}

Waveport::Symmetry
Waveport::getSymZX() const {
	return SymZX;
}

void
Waveport::printInfo() const {
	cout << "--- Waveport info ---" << endl;
	cout << "- Assigned on " << elem.size() << " elements." << endl;
	if (input) {
		cout << "- Is input." << endl;
	} else {
		cout << "- Is output." << endl;
	}
	cout << "- Gaussian spread: " << spread << endl;
	cout << "- Gaussian delay: "	<< delay << endl;
}
