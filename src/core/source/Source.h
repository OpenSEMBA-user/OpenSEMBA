#pragma once

#include "geometry/element/Group.h"

#include "magnitude/Magnitude.h"
#include "magnitude/Numerical.h"
#include "filesystem/Project.h"

#include "class/Class.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

namespace SEMBA {
namespace Source {

class Source;
    typedef Class::Identification<Source> Id;

class Source : public virtual Class::Identifiable<Id>,
               public virtual Class::Class {
public:
    typedef Geometry::ElemView Target;

    Source() = default;
    Source(const std::unique_ptr<Magnitude::Magnitude>&, const Target&);
    Source(const Source&);

    virtual ~Source() = default;

    Source& operator=(const Source&);
    
    virtual std::unique_ptr<Source> clone() const = 0;

    virtual std::string getName() const = 0;

    void convertToNumerical(const FileSystem::Project& file,
                            const Math::Real step,
                            const Math::Real finalTime);
    
    Magnitude::Numerical exportToFile(const FileSystem::Project& file,
                                       const Math::Real step,
                                       const Math::Real finalTime) const;
    const Magnitude::Magnitude* getMagnitude() const { return magnitude_.get(); }

    Target getTarget() const { return target_; }
    void setTarget(const Target& target) { target_ = target; }

protected:
    std::unique_ptr<Magnitude::Magnitude> magnitude_;
    Target target_;
};

} /* namespace Source */
} /* namespace SEMBA */


