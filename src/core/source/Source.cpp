

#include "Source.h"

#include <iostream>

namespace SEMBA {
namespace Source {

Base::Base() {
    magnitude_ = nullptr;
}

Base::Base(const Magnitude::Magnitude* magnitude) {
    magnitude_ = magnitude;
}

Base::Base(const Base& rhs) {
    if (rhs.magnitude_ != nullptr) {
        magnitude_ = rhs.magnitude_->cloneTo<Magnitude::Magnitude>();
    } else {
        magnitude_ = rhs.magnitude_;
    }
}

Base::~Base() {
    if (magnitude_ != nullptr) {
        delete magnitude_;
    }
}

std::string Base::getMagnitudeFilename() const {
    const Magnitude::Numerical* mag =
            dynamic_cast<const Magnitude::Numerical*>(magnitude_);
    if (mag != nullptr) {
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

const Magnitude::Magnitude* Base::getMagnitude() const {
    return magnitude_;
}

} /* namespace Source */
} /* namespace SEMBA */
