

#pragma once

#include "Waveguide.h"
#include "physicalModel/Bound.h"

namespace SEMBA {
namespace Source {
namespace Port {

class WaveguideRectangular: public Waveguide {
public:
    WaveguideRectangular(
            Magnitude::Magnitude* magnitude,
            const Geometry::Element::Group<const Geometry::Surf>& elem,
            const ExcitationMode excMode,
            const std::pair<size_t,size_t> mode);
    WaveguideRectangular(const WaveguideRectangular&);
    virtual ~WaveguideRectangular();

    SEMBA_CLASS_DEFINE_CLONE(WaveguideRectangular);

    void set(const Geometry::Element::Group<const Geometry::Elem>&);

    const std::string& getName() const;
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

