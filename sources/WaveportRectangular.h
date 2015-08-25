/*
 * WaveportRectangular.h
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_SOURCES_WAVEPORTRECTANGULAR_H_
#define SRC_COMMON_SOURCES_WAVEPORTRECTANGULAR_H_

#include "../sources/Waveport.h"

class WaveportRectangular: public Waveport {
public:
    WaveportRectangular(
            const Magnitude* magnitude,
            const GroupElements<const Surf>& elem,
            const ExcitationMode excMode,
            const pair<UInt,UInt> mode);
    WaveportRectangular(const WaveportRectangular&);
    virtual ~WaveportRectangular();

    DEFINE_CLONE(WaveportRectangular);

    void set(const GroupElements<const Elem>&);

    bool hasSameProperties(const EMSourceBase& rhs) const;
    virtual CVecR3 getWeight(
            const CVecR3& pos,
            const BoundTerminations& sym) const;

    Real getWidth(const BoundTerminations& sym) const;
    Real getHeight(const BoundTerminations& sym) const;

    CVecR3 getOrigin(const BoundTerminations& sym) const;
private:
    BoxR3 box_; // This has been included for performance.
};

#endif /* SRC_COMMON_SOURCES_WAVEPORTRECTANGULAR_H_ */
