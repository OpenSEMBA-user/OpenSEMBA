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

#include "Source.h"

#include <iostream>

namespace SEMBA {
namespace Source {

Base::Base() {
    magnitude_ = NULL;
}

Base::Base(const Magnitude::Magnitude* magnitude) {
    magnitude_ = magnitude;
}

Base::Base(const Base& rhs) {
    if (rhs.magnitude_ != NULL) {
        magnitude_ = rhs.magnitude_->cloneTo<Magnitude::Magnitude>();
    } else {
        magnitude_ = rhs.magnitude_;
    }
}

Base::~Base() {
    if (magnitude_ != NULL) {
        delete magnitude_;
    }
}

bool Base::check() const {
// TODO Redo this function
    throw std::logic_error("Source::Base::check() not implemented");
}

bool Base::hasSameProperties(const Base& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    return (*magnitude_ == *rhs.magnitude_);
}

std::string Base::getMagnitudeFilename() const {
    const Magnitude::Numerical* mag =
            dynamic_cast<const Magnitude::Numerical*>(magnitude_);
    if (mag != NULL) {
        return mag->getFilename();
    }
    return std::string();
}

void Base::convertToNumerical(const FileSystem::Project& file,
                              const Math::Real step,
                              const Math::Real finalTime) {
    if(magnitude_->is<Magnitude::Numerical>()) {
        return;
    }
    const Magnitude::Magnitude* orig = magnitude_;
    magnitude_ = new Magnitude::Numerical(file, *magnitude_, step, finalTime);
    delete orig;
}

Magnitude::Numerical* Base::exportToFile(const FileSystem::Project& file,
                                         const Math::Real step,
                                         const Math::Real finalTime) const {
    if(magnitude_->is<Magnitude::Numerical>()) {
        return magnitude_->cloneTo<Magnitude::Numerical>();
    }
    return new Magnitude::Numerical(file, *magnitude_, step, finalTime);
}

void Base::printInfo() const {
    std::cout << " --- EMSource Base Info ---" << std::endl;
    if (magnitude_ != NULL) {
        magnitude_->printInfo();
    } else {
        std::cout << "No magnitude defined." << std::endl;
    }
}

const Magnitude::Magnitude* Base::getMagnitude() const {
    return magnitude_;
}

} /* namespace Source */
} /* namespace SEMBA */
