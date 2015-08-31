/*
 * PMMultiportRLC.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMMultiportRLC.h"

PMMultiportRLC::PMMultiportRLC(
        const MatId id,
        const string name,
        const PMMultiport::Type type,
        const Real resistance,
        const Real inductance,
        const Real capacitance) : PMMultiport(id, name) {
    type_ = type;
    R_ = resistance;
    L_ = inductance;
    C_ = capacitance;
}

PMMultiportRLC::~PMMultiportRLC() {

}

Real
PMMultiportRLC::getR() const {
    return R_;
}

Real
PMMultiportRLC::getL() const {
    return L_;
}

Real
PMMultiportRLC::getC() const {
    return C_;
}

void
PMMultiportRLC::printInfo() const {
    cout<< " --- Multiport RLC Info --- " << endl;
    PMMultiport::printInfo();
    cout<< " Type: " << getTypeStr() << endl;
    cout<< " Resistance: " << R_ << endl;
    cout<< " Inductance: " << L_ << endl;
    cout<< " Capacitance: " << C_ << endl;
}
