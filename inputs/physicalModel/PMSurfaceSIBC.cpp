/*
 * PMSurface.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMSurfaceSIBC.h"

PMSurfaceSIBC::~PMSurfaceSIBC() {

}

PMSurfaceSIBC::PMSurfaceSIBC(
        const unsigned int id, const string& name,
        const StaMatrix<double,2,2>& Zinfinite,
        const StaMatrix<double,2,2>& Zstatic,
        const vector<double>& pole,
        const vector<StaMatrix<double,2,2> >& Z) :
                     PMSurface(id, name) {
    ZInfinity_ = Zinfinite;
    ZStatic_ = Zstatic;
    pole_ = pole;
    Z_ = Z;
}

PMSurfaceSIBC&
PMSurfaceSIBC::operator=(
        const PMSurfaceSIBC &rhs) {
    if (this == &rhs) {
        return *this;
    }
    // Copies values to object.
    PMSurface::operator=(rhs);
    ZInfinity_ = rhs.ZInfinity_;
    ZStatic_ = rhs.ZStatic_;
    pole_ = rhs.pole_;
    Z_ = rhs.Z_;
    return *this;
}

void
PMSurfaceSIBC::setZInfinity(const StaMatrix<double,2,2>& ZInf) {
    ZInfinity_ = ZInf;
}

void
PMSurfaceSIBC::setZStatic(const StaMatrix<double,2,2>& ZSta) {
    ZStatic_ = ZSta;
    return;
}

void
PMSurfaceSIBC::addPole(
        const double vP, const StaMatrix<double,2,2>& Z) {
    pole_.push_back(vP);
    Z_.push_back(Z);
    return;
}

void
PMSurfaceSIBC::printInfo() const {
    cout << "----- Physical model information -----"   << endl;
    PMSurface::printInfo();
    cout << "Type: Surface Impedance Boundary Condition" << endl;
    cout << "Impedance @ infininite freq.:";
    ZInfinity_.printInfo();
    cout << "Impedance @ zero freq.:";
    ZStatic_.printInfo();
    if (pole_.size()) {
        cout << "Number of poles: " << pole_.size() << endl;
        cout << "# Pole Z11 Z12 Z21 Z22 " << endl;
    }
    for (register unsigned int i = 0; i < pole_.size(); i++) {
        cout << i << " "
                << pole_[i] << " "
                << Z_[i](0,0) << " " << Z_[i](0,1) << " "
                << Z_[i](1,0) << " " << Z_[i](1,1) << endl;
    }
}

Condition::Type
PMSurfaceSIBC::getConditionType() const {
    return Condition::sibc;
}
