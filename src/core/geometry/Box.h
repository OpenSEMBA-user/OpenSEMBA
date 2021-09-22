

#pragma once

#include <algorithm>
#include <exception>
#include <utility>

#include "math/vector/Cartesian.h"
#include "Grid.h"

namespace SEMBA {
namespace Geometry {

template<std::size_t D> class Grid;

template <class T, std::size_t D>
class Box {
    typedef Math::Vector::Cartesian<T,D> CVecTD;
public:
    Box();
    Box(const std::pair<CVecTD,CVecTD>& boundsMinMax);
    Box(const CVecTD& min, const CVecTD& max);

	template <typename T2>
	Box<T,D>(const Box<T2, D>& rhs) {
		for (size_t i = 0; i < D; ++i) {
			min_(i) = (T)rhs.getMin()(i);
			max_(i) = (T)rhs.getMax()(i);
		}
	}

    virtual ~Box();

    Box<T,D>& operator= (const Box<T,D>& lBoxMin);
	
	template <typename T2>
	Box<T,D>& operator= (const Box<T2, D>& rhs) {
		for (size_t i = 0; i < D; ++i) {
			min_(i) = (T) rhs.getMin()(i);
			max_(i) = (T) rhs.getMax()(i);
		}
		return *this;
	}

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

    std::size_t numberOfDifferentCoords() const;
};

template <class T, std::size_t D>
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

typedef Box<Math::Real,3> BoxR3;
typedef Box<Math::Int ,2> BoxI2;
typedef Box<Math::Int, 3> BoxI3;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Box.hpp"

