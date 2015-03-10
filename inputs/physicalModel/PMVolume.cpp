/*
 * PMVolume.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolume.h"

PMVolume::PMVolume(const MatId id, const string name,
        const Real rEps, const Real rMu) : PhysicalModel(id, name) {
	rEps_ = rEps;
	rMu_ =  rMu;
}

PMVolume::~PMVolume() {

}

ClassBase* PMVolume::clone() const {
   return new PMVolume(*this);
}

Real PMVolume::getImpedance() const {
    if (rEps_ <= 0.0) {
        return numeric_limits<Real>::infinity();
    }
    return sqrt((rMu_ * Constants::mu0) / (rEps_ * Constants::eps0));
}

Real PMVolume::getAdmitance() const {
    if (rMu_ <= 0.0) {
        return numeric_limits<Real>::infinity();
    }
    return (1.0 / getImpedance());
}

inline Real PMVolume::getRelativePermittivity() const {
    return rEps_;
}

inline Real PMVolume::getRelativePermeability() const {
    return rMu_;
}

inline Real PMVolume::getElectricConductivity() const {
    return (Real) (0.0);
}

inline Real PMVolume::getMagneticConductivity() const {
    return (Real) (0.0);
}

inline bool PMVolume::isVacuum() const {
    return (rEps_ == 1.0 && rMu_ == 1.0);
}

inline bool PMVolume::isVolumic() const {
    return true;
}

inline bool PMVolume::isClassic() const {
    return true;
}

inline bool PMVolume::isDispersive() const {
    return false;
}

inline bool PMVolume::isSimplyConductive() const {
    return false;
}

void
PMVolume::printInfo() const {
	cout << "--- PMVolume info ---" << endl;
	PhysicalModel::printInfo();
    cout << "Rel. permittivity @ inft freq: " << rEps_ << endl;
    cout << "Rel. permeability @ inft freq: " << rMu_ << endl;
	cout << "Type: " << "Classical material" << endl;
}

Real PMVolume::getPermittivity() const {
    return (rEps_ * Constants::eps0);
}

Real PMVolume::getPermeability() const {
    return (rMu_ * Constants::mu0);
}
