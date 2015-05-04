/*
 * WaveportRectangular.h
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_ELECTROMAGNETICSOURCES_WAVEPORTRECTANGULAR_H_
#define SRC_COMMON_ELECTROMAGNETICSOURCES_WAVEPORTRECTANGULAR_H_

#include "Waveport.h"

class WaveportRectangular: public Waveport {
public:
    WaveportRectangular(
            const Magnitude* magnitude,
            const GroupElements<const Surf>& elem,
            const ExcitationMode excMode,
            const pair<UInt,UInt> mode);
    virtual ~WaveportRectangular();

    DEFINE_CLONE(WaveportRectangular);

    bool hasSameProperties(const EMSourceBase& rhs) const;
    virtual vector<CVecR3> getElectricWeights(
            vector<CVecR3> pos,
            const BoundTerminations& symmetries) const;

    virtual vector<CVecR3> toLocalAxis(const vector<CVecR3>& rhs) const;
    virtual vector<CVecR3> toGlobalAxis(const vector<CVecR3>& rhs) const;

    Real getWidth() const;
    Real getHeight() const;
};

#endif /* SRC_COMMON_ELECTROMAGNETICSOURCES_WAVEPORTRECTANGULAR_H_ */
