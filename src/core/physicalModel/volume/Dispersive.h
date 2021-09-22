

#pragma once

#include <complex>
#include <exception>
#include <utility>

#include "filesystem/Project.h"

#include "Volume.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

typedef std::pair<std::complex<Math::Real>,
                  std::complex<Math::Real>> PoleResidue;

class Dispersive : public virtual Volume {
public:
    Dispersive(const Id id,
                     const std::string& name,
                     const Math::Real rEps,
                     const Math::Real rMu,
                     const Math::Real elecCond,
                     const Math::Real magnCond,
                     const std::vector<PoleResidue>& poleResidue =
                        std::vector<PoleResidue>());
    Dispersive(const Id id,
                     const std::string& name,
                     const FileSystem::Project& file);
    Dispersive(const Dispersive& rhs);
    virtual ~Dispersive();

    SEMBA_CLASS_DEFINE_CLONE(Dispersive);

    std::size_t getPoleNumber() const;
    std::complex<Math::Real> getPole(std::size_t p) const;
    std::complex<Math::Real> getResidue(std::size_t p) const;
    virtual Math::Real getElectricConductivity() const;

    bool isClassic() const;
    bool isSimplyConductive() const;
    bool isDispersive() const;

    void printInfo() const;
    const FileSystem::Project getFile() const;

protected:
    Math::Real rEpsInfty_, rMuInfty_; // @ InftyFreq.
    std::vector<PoleResidue> poleResidue_; // Residues for dispers model. c_p.
    FileSystem::Project file_;
    void addPole(const std::complex<Math::Real>& pole_,
                 const std::complex<Math::Real>& res_);
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

