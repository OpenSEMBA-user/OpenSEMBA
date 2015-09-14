/*
 * WaveportRectangular.cpp
 *
 *  Created on: May 4, 2015
 *      Author: luis
 */

#include "PortWaveguideRectangular.h"

PortWaveguideRectangular::PortWaveguideRectangular(Magnitude* magn,
        const GroupElements<const Surf>& elem,
        const ExcitationMode excMode,
        const pair<UInt,UInt> mode)
:   EMSourceBase(magn),
    GroupElements<const Surf>(elem),
    PortWaveguide(magn, elem, excMode, mode) {

    box_ = this->getBound();

    if (mode.first == 0 && mode.second == 0) {
        printInfo();
        throw Error("At least one mode must be non-zero.");
    }
}

PortWaveguideRectangular::PortWaveguideRectangular(const PortWaveguideRectangular& rhs)
:   PortWaveguide(rhs) {
    box_ = rhs.box_;
}

PortWaveguideRectangular::~PortWaveguideRectangular() {

}

bool PortWaveguideRectangular::hasSameProperties(const EMSourceBase& rhs) const {
    bool res = true;
    res &= rhs.is<PortWaveguideRectangular>();
    res &= PortWaveguide::hasSameProperties(rhs);
    return res;
}

const string& PortWaveguideRectangular::getName() const {
    const static string res = "Rectangular_waveguide_port";
    return res;
}

CVecR3 PortWaveguideRectangular::getWeight(
        const CVecR3& pos,
        const BoundTerminations& sym) const {
    // Return normalized weights for electric field components.
    static const Real pi = acos(-1.0);
    CVecR3 res;
    CVecR3 rPos = pos - getOrigin(sym);
    const Real m = pi * getMode().first / getWidth(sym);
    const Real n = pi * getMode().second / getHeight(sym);
    Real normFactor = m;
    if (n > m) {
        normFactor = n;
    }
    //const Real betaC = sqrt(pow(m,2) + pow(n,2));
    if (getExcitationMode() == PortWaveguide::TE) {
        res(x) =   n * cos(m * rPos(x)) * sin(n * rPos(y)) / normFactor;
        res(y) = - m * sin(m * rPos(x)) * cos(n * rPos(y)) / normFactor;
        res(z) = (Real) 0.0;
    } else {
        res(x) = - m * cos(m * rPos(x)) * sin(n * rPos(y)) / normFactor;
        res(y) = - m * sin(m * rPos(x)) * cos(n * rPos(y)) / normFactor;
        res(z) = (Real) 0.0;
    }
    return res;
}

Real PortWaveguideRectangular::getWidth(const BoundTerminations& sym) const {
    CVecR3 origin = getOrigin(sym);
    CVecR3 max = box_.getMax();
    return max(x) - origin(x);
}

Real PortWaveguideRectangular::getHeight(const BoundTerminations& sym) const {
    CVecR3 origin = getOrigin(sym);
    CVecR3 max = box_.getMax();
    return max(y) - origin(y);
}

void PortWaveguideRectangular::set(
        const GroupElements<const Elem>& constGroupElements) {
    PortWaveguide::set(constGroupElements);
    box_ = this->getBound();
}

CVecR3 PortWaveguideRectangular::getOrigin(const BoundTerminations& sym) const {
    if (sym[x].first != OptionsMesher::pml && sym[x].first != OptionsMesher::pmc) {
        throw Error("Waveport must have PML or PMC boundary in the x lower axis");
    }
    if (sym[y].first != OptionsMesher::pml && sym[y].first != OptionsMesher::pec) {
        throw Error("Waveport must have PML or PEC boundary in the y lower axis");
    }
    CVecR3 min = box_.getMin();
    CVecR3 max = box_.getMax();
    CVecR3 res = min;
    if (sym[x].first == OptionsMesher::pmc) {
        res(x) = - max(x);
    }
    if (sym[y].first == OptionsMesher::pec) {
        res(y) = - max(y);
    }
    return res;
}
