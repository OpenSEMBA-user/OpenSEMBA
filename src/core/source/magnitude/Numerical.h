

#pragma once

#include <iomanip>

#include "Magnitude.h"
#include "filesystem/Project.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

class Numerical : public virtual Magnitude,
                  public virtual FileSystem::Project {
public:
    Numerical();
    Numerical(const FileSystem::Project& filename);
    Numerical(const FileSystem::Project& filename,
              const Magnitude& mag,
              const Math::Real timeStep,
              const Math::Real finalTime);
    virtual ~Numerical();

    SEMBA_CLASS_DEFINE_CLONE(Numerical);

    Numerical& operator=(const Numerical& rhs);

    bool operator==(const Magnitude&) const;
    Math::Real evaluate(const Math::Real time) const;

    void printInfo() const;

private:
    static const std::size_t defaultNumberOfSteps = 1000;


};

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */

