/*
 * WaveportRectangular.h
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_SOURCES_PORTWAVEGUIDERECTANGULAR_H_
#define SRC_COMMON_SOURCES_PORTWAVEGUIDERECTANGULAR_H_

#include "PortWaveguide.h"

class PortWaveguideRectangular: public PortWaveguide {
public:
    PortWaveguideRectangular(
            Magnitude* magnitude,
            const GroupElements<const Surf>& elem,
            const ExcitationMode excMode,
            const pair<UInt,UInt> mode);
    PortWaveguideRectangular(const PortWaveguideRectangular&);
    virtual ~PortWaveguideRectangular();

    DEFINE_CLONE(PortWaveguideRectangular);
    bool hasSameProperties(const EMSourceBase& rhs) const;

    void set(const GroupElements<const Elem>&);

    const string& getName() const;
    Real getWidth(const BoundTerminations& sym) const;
    Real getHeight(const BoundTerminations& sym) const;

    CVecR3 getOrigin(const BoundTerminations& sym) const;
    CVecR3 getWeight(
            const CVecR3& pos,
            const BoundTerminations& sym) const;
private:
    BoxR3 box_; // This has been included for performance.
};

#endif /* SRC_COMMON_SOURCES_PORTWAVEGUIDERECTANGULAR_H_ */
