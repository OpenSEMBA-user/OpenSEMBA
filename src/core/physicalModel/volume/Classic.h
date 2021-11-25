

#pragma once

#include <limits>

#include "Volume.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

class Classic : public virtual Volume {
public:
    Classic(const Id matId,
                  const std::string& name,
                  const Math::Real relativePermittivity = 1.0,
                  const Math::Real relativePermeability = 1.0,
                  const Math::Real electricConductivity = 0.0,
                  const Math::Real magneticConductivity = 0.0);
    Classic(const Classic&);
    virtual ~Classic();

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<Classic>(*this);
    }

    Math::Real getRelativePermittivity() const;
    Math::Real getPermittivity() const;
    Math::Real getRelativePermeability() const;
    Math::Real getPermeability() const;
    Math::Real getImpedance() const;
    Math::Real getAdmitance() const;
    Math::Real getElectricConductivity() const;
    Math::Real getMagneticConductivity() const;
    bool isVacuum() const;

private:
    Math::Real rEps_; // Rel. permittivity @ infte. freq.
    Math::Real rMu_; // Rel. permeability @ infte. freq.
    Math::Real electricConductivity_;
    Math::Real magneticConudctivity_;
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

