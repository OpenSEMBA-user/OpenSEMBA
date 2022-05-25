

#pragma once

#include "Waveguide.h"
#include "physicalModel/Bound.h"

namespace SEMBA {
namespace Source {
namespace Port {

class WaveguideRectangular: public Waveguide {
public:
    WaveguideRectangular(
            const std::unique_ptr<Magnitude::Magnitude>& magnitude,
            const Target& elem,
            const ExcitationMode excMode,
            const std::pair<size_t,size_t> mode);
    WaveguideRectangular(const WaveguideRectangular&);
    virtual ~WaveguideRectangular();

    virtual std::unique_ptr<Source> clone() const override {
        return std::make_unique<WaveguideRectangular>(*this);
    }

    void set(const Target&);

    std::string getName() const;
    Math::Real getWidth() const;
    Math::Real getHeight() const;

    Math::CVecR3 getOrigin() const;
    Math::CVecR3 getWeight(const Math::CVecR3& pos) const;
private:
    Geometry::BoxR3 box_; // This has been included for performance.
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

