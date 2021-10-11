

#pragma once

#include <array>
#include <complex>

#include "math/Types.h"

#include "Matrix.h"
#include "Dynamic.h"

namespace SEMBA {
namespace Math {
namespace Matrix {

// Static Matrix class. It can't change its size in run time.
template<class T,std::size_t ROWS,std::size_t COLS>
class Static : public Matrix<T> {
public:
   Static();
   Static(const Static<T,ROWS,COLS>& rhs);
   ~Static();
   T operator()(const std::size_t row, const std::size_t col) const;
   T& operator()(const std::size_t row, const std::size_t col);
   T val(const std::size_t ind) const;
   T& val(const std::size_t ind);
   T val(const std::size_t row, const std::size_t col) const;
   T& val(const std::size_t row, const std::size_t col);
   const T* val() const;
   std::size_t nRows() const;
   std::size_t nCols() const;

   Static<T,ROWS,COLS>& operator=(const Static<T,ROWS,COLS>& rhs);
   Static<T,ROWS,COLS>& operator=(const Dynamic<Int>& rhs);
   Static<T,ROWS,COLS>& operator=(const Dynamic<Real>& rhs);

   Static<T,ROWS,COLS>& assign(const Static<Int,ROWS,COLS>& rhs);
   Static<T,ROWS,COLS>& setInDiagonal(const Vector::Cartesian<T,ROWS>& rhs);

   Static<T,ROWS,COLS>& operator+=(const Real rhs);
   Static<T,ROWS,COLS>& operator+=(const Static<T,ROWS,COLS>& rhs);
   Static<T,ROWS,COLS>& operator-=(const Static<T,ROWS,COLS>& rhs);
   Static<T,ROWS,COLS>& operator*=(const T rhs);
   Static<T,ROWS,COLS>& operator/=(const T rhs);
   Static<T,ROWS,COLS> operator*(T rhs) const;
   Vector::Cartesian<T,COLS> operator*(Vector::Cartesian<T,COLS> rhs) const;
   Static<T,ROWS,COLS> operator+(Static<T,ROWS,COLS> &rhs) const;
   Static<T,COLS,ROWS>& invert();
   Static<T,COLS,ROWS> transpose();

   std::array<std::complex<Real>,ROWS> getEigenvalues() const;

   void copy(std::vector<std::vector<T> > values);
   bool operator==(const Static<T,ROWS,COLS>& rhs) const;
   bool operator<(const Static<T,ROWS,COLS>& rhs) const;

private:
   T _val[ROWS*COLS];
};

// Products between different sized StaMatrices.
template<class T, class S,
         std::size_t ROWS, std::size_t COLS, std::size_t NCOLSB>
Static<T,ROWS,NCOLSB> operator*(const Static<T,ROWS,COLS>& lhs,
                                const Static<S,COLS,NCOLSB>& rhs);
// Products between Static and Dynamic
template<class T, std::size_t ROWS, std::size_t COLS>
Dynamic<T> operator*(const Static<T,ROWS,COLS>& lhs,
                     const Dynamic<T>& rhs);

} /* namespace Matrix */
} /* namespace Math */
} /* namespace SEMBA */

#include "Static.hpp"

namespace SEMBA {
namespace Math {

typedef Matrix::Static<Real,2,2> MatR22;
typedef Matrix::Static<Real,3,3> MatR33;
typedef Matrix::Static<Real,4,3> MatR43;
typedef Matrix::Static<Real,4,4> MatR44;
typedef Matrix::Static<std::complex<Real>,2,2> MatC22;
typedef Matrix::Static<std::complex<Real>,3,3> MatC33;

} /* namespace Math */
} /* namespace SEMBA */

