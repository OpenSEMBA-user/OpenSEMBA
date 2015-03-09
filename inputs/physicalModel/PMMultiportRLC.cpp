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
        const double resistance,
        const double inductance,
        const double capacitance) : PMMultiport(id, name) {
    type_ = type;
    R_ = resistance;
    L_ = inductance;
    C_ = capacitance;
}

PMMultiportRLC::~PMMultiportRLC() {

}

ClassBase* PMMultiportRLC::clone() const {
   return new PMMultiportRLC(*this);
}

double
PMMultiportRLC::getR() const {
    return R_;
}

double
PMMultiportRLC::getL() const {
    return L_;
}

double
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
