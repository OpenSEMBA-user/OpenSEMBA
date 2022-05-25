

#pragma once

#include "Source.h"

namespace SEMBA {
namespace Source {

class Dipole : public Source {
public:
    Dipole(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
           const Target& elem,
           Math::Real   length,
           Math::CVecR3 orientation,
           Math::CVecR3 position);
    
    virtual std::unique_ptr<Source> clone() const override {
        return std::make_unique<Dipole>(*this);
    }

    std::string getName() const { return "Dipole"; }
private:
    Math::Real length_;
    Math::CVecR3 orientation_;
    Math::CVecR3 position_;
};

} /* namespace Source */
} /* namespace SEMBA */

