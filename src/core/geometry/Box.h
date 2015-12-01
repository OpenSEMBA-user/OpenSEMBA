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

#ifndef SEMBA_GEOMETRY_BOX_BOX_H_
#define SEMBA_GEOMETRY_BOX_BOX_H_

#include <exception>
#include <utility>

#include "Types.h"
#include "math/vector/Cartesian.h"
#include "Grid.h"

namespace SEMBA {
namespace Geometry {

template<Size D> class Grid;

template <class T, Size D>
class Box {
    typedef Math::Vector::Cartesian<T,D> CVecTD;
public:
    Box();
    Box(const std::pair<CVecTD,CVecTD>& boundsMinMax);
    Box(const CVecTD& min, const CVecTD& max);
    virtual ~Box();

    Box<T,D>& operator= (const Box<T,D>& lBoxMin);

    bool operator>(const Box<T,D>& lBoxMin) const;
    bool operator<(const Box<T,D>& lBoxMax) const;
    bool operator>=(const Box<T,D>& lBoxMin) const;
    bool operator<=(const Box<T,D>& lBoxMax) const;
    bool operator==(const Box<T,D>& lBoxMin) const;

    void operator+=(const Box<T,D>& lBoxSource);
    void operator+(const Box<T,D>& lBoxSource);

    void operator<<(const CVecTD& p);
    void operator<<(const Box<T,D>& p);

    bool isPoint  () const { return (numberOfDifferentCoords() == 0); }
    bool isLine   () const { return (numberOfDifferentCoords() == 1); }
    bool isSurface() const { return (numberOfDifferentCoords() == 2); }
    bool isVolume () const { return (numberOfDifferentCoords() == 3); }

    Math::Constants::CartesianAxis getDirection() const;
    Math::Constants::CartesianAxis getNormal   () const;

    Box<T,D> intersect(const Box<T,D>& rhs) const;
    bool isIntersected (const Box<T,D>& lBox) const;
    bool isInnerPoint(const CVecTD& point) const;

    inline CVecTD getMin() const;
    inline CVecTD getMax() const;
    inline CVecTD getLength() const;

    std::vector<CVecTD> getPos() const;
    std::vector<CVecTD> getPosOfBound(Math::Constants::CartesianAxis  d,
                                      Math::Constants::CartesianBound p) const;
    Box<T,D> getBoundAsBox(Math::Constants::CartesianAxis  d,
                           Math::Constants::CartesianBound p) const;
    CVecTD getBound(Math::Constants::CartesianBound p) const;
    std::vector<Box<T,D>> chop(const CVecTD step = CVecTD(1,1,1)) const;
    std::vector<Box<T,D>> chop(const Grid<D>& grid) const;

    void set(const std::pair<CVecTD,CVecTD>& boundsMinMax);
    Box<T,D>& setInfinity();
    void scale(const Math::Real factor);

    void printInfo() const;
private:
    CVecTD min_, max_;

    void setDefaultValues() {
        min_.setPlusInfty();
        max_.setMinusInfty();
    };

    Size numberOfDifferentCoords() const;
};

template <class T, Size D>
std::ostream& operator<<(std::ostream& os, const Box<T,D>& rhs) {
   return os << "Min: " << rhs.getMin() << ", Max: " << rhs.getMax();
}

namespace Error {
namespace Box {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

class NotPoint : public Error {
public:
    NotPoint() {}
    virtual ~NotPoint() throw() {}

    const char* what() const throw() { return "Box is not a Point"; }
};

class NotLine : public Error {
public:
    NotLine() {}
    virtual ~NotLine() throw() {}

    const char* what() const throw() { return "Box is not a Line"; }
};

class NotSurface : public Error {
public:
    NotSurface() {}
    virtual ~NotSurface() throw() {}

    const char* what() const throw() { return "Box is not a Surface"; }
};

class NotVolume : public Error {
public:
    NotVolume() {}
    virtual ~NotVolume() throw() {}

    const char* what() const throw() { return "Box is not a Volume"; }
};
} /* namespace Box */
} /* namespace Error */
} /* namespace Geometry */
} /* namespace SEMBA */

#include "Box.hpp"

namespace SEMBA {
namespace Geometry {

typedef Box<Math::Real,3> BoxR3;
typedef Box<Math::Int ,2> BoxI2;
typedef Box<Math::Int, 3> BoxI3;

} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_BOX_BOX_H_ */
