// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
// File: electromagneticsource.h

#ifndef SEMBA_SOURCE_SOURCE_H_
#define SEMBA_SOURCE_SOURCE_H_

#include "geometry/element/Group.h"
#include "magnitude/Magnitude.h"
#include "magnitude/Numerical.h"
#include "filesystem/Project.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"

namespace SEMBA {
namespace Source {

class Base : public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Shareable,
             public virtual Class::Printable {
public:
    Base();
    Base(const Magnitude::Magnitude* magnitude);
    Base(const Base& rhs);
    virtual ~Base();

    virtual Geometry::Element::Group<const Geometry::Elem> elems() const = 0;

    virtual void set(
            const Geometry::Element::Group<const Geometry::Elem>&) = 0;
    virtual void add(
            const Geometry::Element::Group<const Geometry::Elem>&) = 0;

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

    virtual void printInfo() const;


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
    virtual void printInfo() const;
};

} /* namespace Source */
} /* namespace SEMBA */

#include "Source.hpp"

#endif /* SEMBA_SOURCE_SOURCE_H_ */
