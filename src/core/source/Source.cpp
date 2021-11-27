#include "Source.h"

#include <iostream>

namespace SEMBA {
namespace Source {

Source::Source(const std::unique_ptr<Magnitude::Magnitude>& magnitude, const Target& target) :
    target_(target),
    magnitude_(magnitude->clone())
{
}

Source::Source(const Source& rhs) 
{
    target_ = rhs.target_;
    if (rhs.magnitude_ != nullptr) {
        magnitude_ = rhs.magnitude_->clone();
    }
}

Source& Source::operator=(const Source& rhs) const
{
    Source res;
    target_ = rhs.target_;
    if (rhs.magnitude_ != nullptr) {
        magnitude_ = rhs.magnitude_->clone();
    }
}


void Source::convertToNumerical(
    const FileSystem::Project& file,
    const Math::Real step,
    const Math::Real finalTime) 
{
    if(magnitude_->is<Magnitude::Numerical>()) {
        return;
    }
    auto newMagnitude = std::make_unique<Magnitude::Numerical>(file, *magnitude_, step, finalTime);
    magnitude_ = std::move(newMagnitude);
}

Magnitude::Numerical Source::exportToFile(const FileSystem::Project& file,
                                         const Math::Real step,
                                         const Math::Real finalTime) const {
    if(magnitude_->is<Magnitude::Numerical>()) {
        return Magnitude::Numerical(*magnitude_->castTo<Magnitude::Numerical>());
    }
    return Magnitude::Numerical(file, *magnitude_, step, finalTime);
}

} /* namespace Source */
} /* namespace SEMBA */
