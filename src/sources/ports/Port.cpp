/*
 * Port.cpp
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#include "Port.h"


Port::Port(Magnitude* magnitude, const GroupElements<const Surf>& elem) :
    EMSourceBase(magnitude),
    GroupElements<const Surf>(elem) {

}

Port::Port(const Port& rhs) :
            EMSourceBase(rhs),
            GroupElements<const Surf>(rhs) {
}

Port::~Port() {
}

CVecR3 Port::getNormal() const {
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
