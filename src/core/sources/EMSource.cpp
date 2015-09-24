// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
#include "../sources/EMSource.h"

EMSourceBase::EMSourceBase() {
    magnitude_ = NULL;
}

EMSourceBase::EMSourceBase(const Magnitude* magnitude) {
    magnitude_ = magnitude;
}

EMSourceBase::EMSourceBase(const EMSourceBase& rhs) {
    if (rhs.magnitude_ != NULL) {
        magnitude_ = rhs.magnitude_->cloneTo<Magnitude>();
    } else {
        magnitude_ = rhs.magnitude_;
    }
}

EMSourceBase::~EMSourceBase() {
    if (magnitude_ != NULL) {
        delete magnitude_;
    }
}

bool EMSourceBase::check() const {
    return ClassGroupBase<GroupElements<const Elem>>::check("EMSource");
}

bool EMSourceBase::hasSameProperties(const EMSourceBase& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    return (*magnitude_ == *rhs.magnitude_);
}

string EMSourceBase::getMagnitudeFilename() const {
    const MagnitudeNumerical* mag =
            dynamic_cast<const MagnitudeNumerical*>(magnitude_);
    if (mag != NULL) {
        return mag->getFilename();
    }
    return string();
}

void EMSourceBase::convertToNumerical(const ProjectFile& file,
                                      const Real step,
                                      const Real finalTime) {
    if(magnitude_->is<MagnitudeNumerical>()) {
        return;
    }
    const Magnitude* orig = magnitude_;
    magnitude_ = new MagnitudeNumerical(file, *magnitude_, step, finalTime);
    delete orig;
}

MagnitudeNumerical* EMSourceBase::exportToFile(const ProjectFile& file,
                                               const Real step,
                                               const Real finalTime) const {

    if(magnitude_->is<MagnitudeNumerical>()) {
        return magnitude_->cloneTo<MagnitudeNumerical>();
    }
    return new MagnitudeNumerical(file, *magnitude_, step, finalTime);
}

void EMSourceBase::printInfo() const {
    cout << " --- EMSource Base Info ---" << endl;
    if (magnitude_ != NULL) {
        magnitude_->printInfo();
    } else {
        cout << "No magnitude defined." << endl;
    }
}

const Magnitude* EMSourceBase::getMagnitude() const {
    return magnitude_;
}
