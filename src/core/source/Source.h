
// File: electromagneticsource.h

#pragma once

#include "geometry/element/Group.h"
#include "magnitude/Magnitude.h"
#include "magnitude/Numerical.h"
#include "filesystem/Project.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"

namespace SEMBA {
namespace Source {

class Base : public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Shareable {
public:
    Base();
    Base(const Magnitude::Magnitude* magnitude);
    Base(const Base& rhs);
    virtual ~Base();

    virtual Geometry::Element::Group<const Geometry::Elem> elems() const = 0;

    virtual void set(const Geometry::Element::Group<const Geometry::Elem>&) = 0;
    virtual void add(const Geometry::Element::Group<const Geometry::Elem>&) = 0;

    template<class T>
    bool magnitudeIs() const {
        return magnitude_->is<T>();
    }

    virtual std::string getMagnitudeFilename() const;
    virtual const std::string& getName() const = 0;

    void convertToNumerical(const FileSystem::Project& file,
                            const Math::Real step,
                            const Math::Real finalTime);
    Magnitude::Numerical* exportToFile(const FileSystem::Project& file,
                                       const Math::Real step,
                                       const Math::Real finalTime) const;
    const Magnitude::Magnitude* getMagnitude() const;

private:
    const Magnitude::Magnitude* magnitude_;
};

template<class T>
class Source : public virtual Base,
               public virtual Geometry::Element::Group<const T> {
public:
    Source() {}
    virtual ~Source() {}

    void set(const Geometry::Element::Group<const Geometry::Elem>&);
    void add(const Geometry::Element::Group<const Geometry::Elem>&);

    virtual Source* clone() const = 0;
    Geometry::Element::Group<const Geometry::Elem> elems() const {
        return *this;
    }

    bool isSimilar(const SEMBA::Source::Base& rhs) const;
};

} /* namespace Source */
} /* namespace SEMBA */

#include "Source.hpp"

