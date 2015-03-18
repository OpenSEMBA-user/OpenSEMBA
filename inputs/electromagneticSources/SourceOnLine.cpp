/*
 * Nodal.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#include "SourceOnLine.h"

SourceOnLine::SourceOnLine(
 const ElementsGroup<Line<> >& elem,
 const Magnitude* magnitude,
 const Type& sourceType,
 const Hardness& sourceHardness) :
         EMSource(magnitude), ElementsGroup<Line<> >(elem) {
	type_ = sourceType;
    hardness_ = sourceHardness;
}

ClassBase* SourceOnLine::clone() const {
    return new SourceOnLine(*this);
}

SourceOnLine::Type
SourceOnLine::getType() const {
	return type_;
}

SourceOnLine::Hardness
SourceOnLine::getHardness() const {
	return hardness_;
}

void
SourceOnLine::printInfo() const {
	cout<< " --- Nodal info --- " << endl;
	EMSource::printInfo();
	cout<< "Type: " << getTypeStr() << endl;
    cout<< "Hardness: " << hardnessToStr() << endl;
}

string
SourceOnLine::getTypeStr() const {
	switch (type_) {
	case electric:
		return "Electric Field";
	default:
		return "Magnetic Field";
	}
}

string
SourceOnLine::hardnessToStr() const {
	switch (type_) {
	case hard:
		return "Hard";
	default:
		return "Soft";
	}
}
