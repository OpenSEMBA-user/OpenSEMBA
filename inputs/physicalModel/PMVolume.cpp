/*
 * PMVolume.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolume.h"

PMVolume::PMVolume(const uint id, const string name,
        const double rEps, const double rMu) : PhysicalModel(id, name) {
	rEps_ = rEps;
	rMu_ =  rMu;
}

PMVolume::~PMVolume() {

}

double PMVolume::getImpedance() const {
    if (rEps_ <= 0.0) {
        return numeric_limits<double>::infinity();
    }
    return sqrt((rMu_ * Constants::mu0) / (rEps_ * Constants::eps0));
}

double PMVolume::getAdmitance() const {
    if (rMu_ <= 0.0) {
        return numeric_limits<double>::infinity();
    }
    return (1.0 / getImpedance());
}

inline double PMVolume::getRelativePermittivity() const {
    return rEps_;
}

inline double PMVolume::getRelativePermeability() const {
    return rMu_;
}

inline double PMVolume::getElectricConductivity() const {
    return (double) (0.0);
}

inline double PMVolume::getMagneticConductivity() const {
    return (double) (0.0);
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
