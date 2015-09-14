/*
 * PortTEMCoaxial.h
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_
#define SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_

#include "PortTEM.h"

class PortTEMCoaxial : public PortTEM {
public:
    PortTEMCoaxial(
            Magnitude* magnitude,
            const GroupElements<const Surf>& elem,
            const ExcitationMode excMode,
            const CVecR3& origin,
            const Real innerRadius,
            const Real outerRadius);
    PortTEMCoaxial(const PortTEMCoaxial& rhs);
    virtual ~PortTEMCoaxial();

    DEFINE_CLONE(PortTEMCoaxial);
    bool hasSameProperties(const PortTEMCoaxial& rhs) const;

    const string& getName() const;

    CVecR3 getOrigin(const BoundTerminations& sym = BoundTerminations()) const;
    CVecR3 getWeight(
            const CVecR3& pos,
            const BoundTerminations& sym = BoundTerminations()) const;
private:
    CVecR3 origin_;
    Real innerRadius_, outerRadius_;
};

#endif /* SRC_COMMON_SOURCES_PORTTEMCOAXIAL_H_ */
