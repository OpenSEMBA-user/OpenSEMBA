

#include "Numerical.h"

#include "math/function/LinearInterpolation.h"
#include "math/util/Real.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

Numerical::Numerical() {

}

Numerical::Numerical(const FileSystem::Project& file)
:   Magnitude(
        new Math::Function::LinearInterpolation<Math::Real,Math::Real>(file)),
    FileSystem::Project(file) {
}

Numerical::Numerical(const FileSystem::Project& file,
                     const Magnitude& mag,
                     const Math::Real timeStep,
                     const Math::Real finalTime)
:   FileSystem::Project(file) {

    if(mag.is<Numerical>()) {
        operator=(*mag.castTo<Numerical>());
        return;
    }
    std::size_t nSteps;
    if (timeStep != 0.0) {
        nSteps = (std::size_t)std::abs(finalTime / timeStep);
    } else {
        nSteps = defaultNumberOfSteps;
        std::cerr << "WARNING @ Numerical: "
                  << "Attempting to build a "
                  << "numerical magnitude with a 0.0 step."
                  << "Using default number of steps instead: " << nSteps
                  << std::endl;
        mag.printInfo();
    }
    std::ofstream out;
    out << std::scientific;
    out.precision(10);
    out.open(file.c_str());

    Math::Real time = 0.0;
    for (std::size_t i = 0; i < nSteps; i++) {
        // Determines if neigh values are aligned with current.
        std::vector<std::pair<Math::Real,Math::Real>> preAndPost;
        const Math::Real tPre = time - timeStep;
        const Math::Real tPost = time + timeStep;
        preAndPost.push_back(
            std::pair<Math::Real,Math::Real>(tPre, mag.evaluate(tPre)));
        preAndPost.push_back(
            std::pair<Math::Real,Math::Real>(tPost, mag.evaluate(tPost)));
        const Math::Real interpolated =
            Math::Function::LinearInterpolation<Math::Real,Math::Real>(
                preAndPost)(time);
        const Math::Real current = mag.evaluate(time);
        bool isAligned = Math::Util::equal(current, interpolated,
                0.0, std::numeric_limits<Math::Real>::epsilon());
        if (!isAligned) {
            out << std::setw(16) << std::setfill(' ') << time 
                << std::setw(18) << std::setfill(' ') << current 
                << std::endl;
        }
        //
        time += timeStep;
    }
    out.close();

    Magnitude::operator=(
        Magnitude(
            new Math::Function::LinearInterpolation<Math::Real,Math::Real>(
                file)));
}

Numerical::~Numerical() {

}

Numerical& Numerical::operator=(
        const Numerical& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Magnitude::operator=(rhs);
    FileSystem::Project::operator=(rhs);

    return *this;
}

bool Numerical::operator==(const Magnitude& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    bool areEqual = true;
    areEqual &= Magnitude::operator==(rhs);
    const Numerical* rhsPtr = rhs.castTo<Numerical>();
    areEqual &= (FileSystem::Project::compare(*rhsPtr) == 0);
    return areEqual;
}

Math::Real Numerical::evaluate(const Math::Real time) const {
    throw std::logic_error("Numerical::evaluate not implemented");
    return 0.0;
}

void Numerical::printInfo() const {
    std::cout << " --- Magnitude Numerical Info --- " << std::endl;
    Magnitude::printInfo();
    FileSystem::Project::printInfo();
}

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */
