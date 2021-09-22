

#pragma once

#include "geometry/Grid.h"
#include "math/vector/Cartesian.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

class Base;
typedef Class::Identification<Base> Id;

class Base : public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Shareable,
             public virtual Class::Printable,
             public virtual Class::Identifiable<Id> {
public:
    Base();
    virtual ~Base();

    virtual bool operator==(const Base& rhs) const;
    virtual bool operator!=(const Base& rhs) const;
};

template <class T, std::size_t D>
class Coordinate : public virtual Base,
                   public virtual Math::Vector::Cartesian<T,D> {
public:
    Coordinate();
    Coordinate(const Id id_, const Math::Vector::Cartesian<T,D>& pos);
    explicit Coordinate(const Math::Vector::Cartesian<T,D>& pos);
    Coordinate(const Coordinate& rhs);
    virtual ~Coordinate();

    Coordinate<T,D>* clone() const;

    Coordinate& operator=(const Coordinate& rhs);

    bool operator==(const Base& rhs) const;
    bool operator!=(const Base& rhs) const;

    virtual bool isStructured(const Grid<D>&,
                              const Math::Real = Grid<D>::tolerance) const;

    Math::Vector::Cartesian<T,D>&       pos()       { return *this; }
    const Math::Vector::Cartesian<T,D>& pos() const { return *this; }

    virtual Coordinate<Math::Int ,D>* toStructured  (const Grid<D>&) const;
    virtual Coordinate<Math::Real,D>* toUnstructured(const Grid<D>&) const;

    void printInfo() const;
};

} /* namespace Coordinate */

typedef Coordinate::Id                       CoordId;
typedef Coordinate::Base                     Coord;
typedef Coordinate::Coordinate<Math::Real,3> CoordR3;
typedef Coordinate::Coordinate<Math::Int ,3> CoordI3;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Coordinate.hpp"

