/*
 * WaveportRectangular.cpp
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#include "WaveportRectangular.h"

WaveportRectangular::WaveportRectangular(const Magnitude* magn,
        const GroupElements<const Surf>& elem,
        const ExcitationMode excMode,
        const pair<UInt,UInt> mode) :
EMSourceBase(magn),
GroupElements<const Surf>(elem),
Waveport(magn, elem, excMode, mode) {
        }

WaveportRectangular::~WaveportRectangular() {
    // TODO Auto-generated destructor stub
}

bool WaveportRectangular::hasSameProperties(const EMSourceBase& rhs) const {
    bool res = true;
    res &= rhs.is<WaveportRectangular>();
    res &= Waveport::hasSameProperties(rhs);
    return res;
}

vector<CVecR3> WaveportRectangular::getElectricWeights(
        vector<CVecR3> pos,
        const BoundTerminations& symmetries) const {
    static const Real pi = acos(-1.0);

    vector<CVecR3> res(pos.size());
    pos = toLocalAxis(pos);
    const Real m = pi * getMode().first / getWidth();
    const Real n = pi * getMode().second / getHeight();
    if (getExcitationMode() == Waveport::TE) {
        for (UInt i = 0; i < pos.size(); i++) {
            res[i](x) = n * cos(m * pos[i](x)) * sin(n * pos[i](y));
            res[i](y) = - m * sin(m * pos[i](x)) * cos(n * pos[i](y));
            res[i](z) = (Real) 0.0;
        }
    } else {
        for (UInt i = 0; i < pos.size(); i++) {
            res[i](x) = - m * cos(m * pos[i](x)) * sin(n * pos[i](y));
            res[i](y) = - m * sin(m * pos[i](x)) * cos(n * pos[i](y));
            res[i](z) = (Real) 0.0;
        }
    }
    return toGlobalAxis(res);
}

vector<CVecR3> WaveportRectangular::toLocalAxis(
        const vector<CVecR3>& rhs) const {
}

vector<CVecR3> WaveportRectangular::toGlobalAxis(
        const vector<CVecR3>& rhs) const {
}

Real WaveportRectangular::getWidth() const {
}

Real WaveportRectangular::getHeight() const {
}
