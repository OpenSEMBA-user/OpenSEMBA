/*
 * PMWireExtremes.cpp
 *
 *  Created on: 15/4/2015
 *      Author: Daniel
 */

#include "PMWireExtremes.h"

PMWireExtremes::PMWireExtremes(const PMWire& wire,
                               const PMMultiport* extremeL,
                               const PMMultiport* extremeR)
:   PMWire(wire) {
    extreme_[0] = extreme_[1] = NULL;
    if (extremeL != NULL) {
        extreme_[0] = extremeL->cloneTo<PMMultiport>();
    }
    if (extremeR != NULL) {
        extreme_[1] = extremeR->cloneTo<PMMultiport>();
    }
}

PMWireExtremes::PMWireExtremes(const PMWireExtremes& rhs)
:   PMWire(rhs) {
    extreme_[0] = extreme_[1] = NULL;
    if (rhs.extreme_[0] != NULL) {
        extreme_[0] = rhs.extreme_[0]->cloneTo<PMMultiport>();
    }
    if (rhs.extreme_[1] != NULL) {
        extreme_[1] = rhs.extreme_[1]->cloneTo<PMMultiport>();
    }
}

PMWireExtremes::~PMWireExtremes() {
    if (extreme_[0] != NULL) {
        delete extreme_[0];
    }
    if (extreme_[1] != NULL) {
        delete extreme_[1];
    }
}

void PMWireExtremes::setExtreme(const UInt i, const PMMultiport* extreme) {
    if (extreme_[i] != NULL) {
        delete extreme_[i];
    }
    extreme_[i] = NULL;
    if (extreme != NULL) {
        extreme_[i] = extreme->cloneTo<PMMultiport>();
    }
}

void PMWireExtremes::printInfo() const {
    cout<< " --- WireExtremes info ---" << endl;
    PMWire::printInfo();
    if (extreme_[0] != NULL) {
        cout << "ExtremeL:" << endl;
        extreme_[0]->printInfo();
    }
    if (extreme_[1] != NULL) {
        cout << "ExtremeR:" << endl;
        extreme_[1]->printInfo();
    }
}
