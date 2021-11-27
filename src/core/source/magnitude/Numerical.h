

#pragma once

#include <iomanip>

#include "Magnitude.h"
#include "filesystem/Project.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

class Numerical : public virtual Magnitude {
public:
    Numerical() = default;
    Numerical(const Numerical&) = default;
    Numerical(const FileSystem::Project& filename);
    Numerical(const FileSystem::Project& filename,
              const Magnitude& mag,
              const Math::Real timeStep,
              const Math::Real finalTime);
    
    std::unique_ptr<Magnitude> clone() const override {
        return std::make_unique<Numerical>(*this);
    }

    bool operator==(const Numerical&) const;
    Math::Real evaluate(const Math::Real time) const;

private:
    static const std::size_t defaultNumberOfSteps = 1000;
    FileSystem::Project file;

};

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */

