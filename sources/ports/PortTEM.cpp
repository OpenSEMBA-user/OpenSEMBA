/*
 * PortTEM.cpp
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#include "PortTEM.h"

PortTEM::~PortTEM() {
    // TODO Auto-generated destructor stub
}

PortTEM::PortTEM(
        Magnitude* magnitude,
        const GroupElements<const Surf>& elem,
        const ExcitationMode excitationMode) : Port(magnitude, elem) {
    excitationMode_ = excitationMode;
}

PortTEM::PortTEM(const PortTEM& rhs) : Port(rhs) {
    excitationMode_ = rhs.excitationMode_;
}

PortTEM::ExcitationMode PortTEM::getExcitationMode() const {
    return excitationMode_;
}
