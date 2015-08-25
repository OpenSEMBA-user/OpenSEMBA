/*
 * WaveportRectangular.cpp
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#include "../sources/WaveportRectangular.h"

WaveportRectangular::WaveportRectangular(const Magnitude* magn,
        const GroupElements<const Surf>& elem,
        const ExcitationMode excMode,
        const pair<UInt,UInt> mode)
:   EMSourceBase(magn),
    GroupElements<const Surf>(elem),
    Waveport(magn, elem, excMode, mode) {

}

WaveportRectangular::WaveportRectangular(const WaveportRectangular& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Surf>(rhs),
    Waveport(rhs) {

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

CVecR3 WaveportRectangular::getWeight(
        const CVecR3& pos,
        const BoundTerminations& sym) const {
    static const Real pi = acos(-1.0);
    CVecR3 res;
    CVecR3 rPos = pos - getOrigin(sym);
    const Real m = pi * getMode().first / getWidth(sym);
    const Real n = pi * getMode().second / getHeight(sym);
    if (getExcitationMode() == Waveport::TE) {
        res(x) =   n * cos(m * rPos(x)) * sin(n * rPos(y));
        res(y) = - m * sin(m * rPos(x)) * cos(n * rPos(y));
        res(z) = (Real) 0.0;
    } else {
        res(x) = - m * cos(m * rPos(x)) * sin(n * rPos(y));
        res(y) = - m * sin(m * rPos(x)) * cos(n * rPos(y));
        res(z) = (Real) 0.0;
    }
    return res;
}

Real WaveportRectangular::getWidth(const BoundTerminations& sym) const {
    CVecR3 origin = getOrigin(sym);
    CVecR3 max = this->getBound().getMax();
    return max(x) - origin(x);
}

Real WaveportRectangular::getHeight(const BoundTerminations& sym) const {
    CVecR3 origin = getOrigin(sym);
    CVecR3 max = this->getBound().getMax();
    return max(y) - origin(y);
}

CVecR3 WaveportRectangular::getOrigin(const BoundTerminations& sym) const {
    if (sym[x].first != OptionsMesher::pml && sym[x].first != OptionsMesher::pmc) {
        throw Error("Waveport must have PML or PMC boundary in the x lower axis");
    }
    if (sym[y].first != OptionsMesher::pml && sym[y].first != OptionsMesher::pec) {
        throw Error("Waveport must have PML or PEC boundary in the y lower axis");
    }
    CVecR3 min = this->getBound().getMin();
    CVecR3 max = this->getBound().getMax();
    CVecR3 res = min;
    if (sym[x].first == OptionsMesher::pmc) {
        res(x) = - max(x);
    }
    if (sym[y].first == OptionsMesher::pec) {
        res(y) = - max(y);
    }
    return res;
}
