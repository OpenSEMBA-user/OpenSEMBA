/*
 * PortTEMCoaxial.h
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_
#define SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_

#include "PortTEM.h"

class PortTEMCoaxial : PortTEM {
public:
    PortTEMCoaxial(
            Magnitude* magnitude,
            const GroupElements<const Surf>& elem,
            const ExcitationMode excMode,
            const Real center,
            const Real innerRadius,
            const Real outerRadius);
    virtual ~PortTEMCoaxial();

    DEFINE_CLONE(PortTEMCoaxial);
    bool hasSameProperties(const EMSourceBase& rhs) const;

    CVecR3 getOrigin(const BoundTerminations& sym = BoundTerminations()) const;
    CVecR3 getWeight(
            const CVecR3& pos,
            const BoundTerminations& sym = BoundTerminations()) const;
private:
    CVecR3 origin_;
    Real innerRadius_, outerRadius_;
};

#endif /* SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_ */
