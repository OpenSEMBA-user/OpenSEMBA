#ifndef STAMATRIX_H_
#define STAMATRIX_H_

#include "MathMatrix.h"
#include "DynMatrix.h"

// Static Matrix class. It can't change its size in run time.
template<class T,UInt NROWS,UInt NCOLS>
class StaMatrix : public MathMatrix<T> {
public:
   StaMatrix();
   StaMatrix(const StaMatrix<T,NROWS,NCOLS>& param);
   ~StaMatrix();
   T
    operator()(const UInt row, const UInt col) const;
   T&
    operator()(const UInt row, const UInt col);
   T
    val(const UInt ind) const;
   T&
    val(const UInt ind);
   T
    val(const UInt row, const UInt col) const;
   T&
    val(const UInt row, const UInt col);
   const T*
    val() const;
   UInt
    nRows() const;
   UInt
    nCols() const;
   StaMatrix<T,NROWS,NCOLS>&
    operator=(const StaMatrix<T,NROWS,NCOLS>& param);
   StaMatrix<T,NROWS,NCOLS>&
    operator=(const DynMatrix<Int>& rhs);
   StaMatrix<T,NROWS,NCOLS>&
    assign(const StaMatrix<Int,NROWS,NCOLS>& param);
   StaMatrix<T,NROWS,NCOLS>&
    operator=(const DynMatrix<Real>& param);
   StaMatrix<T,NROWS,NCOLS>&
    operator+=(const Real param);
   StaMatrix<T,NROWS,NCOLS>&
    operator+=(const StaMatrix<T,NROWS,NCOLS>& param);
   StaMatrix<T,NROWS,NCOLS>&
    operator-=(const StaMatrix<T,NROWS,NCOLS>& param);
   StaMatrix<T,NROWS,NCOLS>&
    operator*=(const T param);
   StaMatrix<T,NROWS,NCOLS>&
    operator/=(const T param);
   StaMatrix<T,NROWS,NCOLS>
    operator*(T param) const;
   StaMatrix<T,NROWS,NCOLS>
    operator+(StaMatrix<T,NROWS,NCOLS> &param) const;
   StaMatrix<T,NCOLS,NROWS>&
    invert();
   StaMatrix<T,NCOLS,NROWS>
    transpose();
   void
    copy(vector<vector<T> > values);
   bool
    operator==(const StaMatrix<T,NROWS,NCOLS>& param) const;
   bool
    operator<(const StaMatrix<T,NROWS,NCOLS>& param) const;
   void
    printInfo() const;
   void
    printInfo(UInt, UInt) const;
private:
   T _val[NROWS*NCOLS];
};
// =========== Not member functions ===========================================
// Products between different sized StaMatrices.
template<class T, class S, UInt NROWS, UInt NCOLS, UInt NCOLSB>
StaMatrix<T,NROWS,NCOLSB>
 operator*(const StaMatrix<T,NROWS,NCOLS>& lhs,
        const StaMatrix<S,NCOLS,NCOLSB>& rhs);
// Products between StaMatrix and DynMatrix
template<class T, UInt NROWS, UInt NCOLS>
DynMatrix<T>
 operator*(const StaMatrix<T,NROWS,NCOLS>& lhs, const DynMatrix<T>& rhs);

#include "StaMatrix.hpp"

typedef StaMatrix<Real,2,2> MatR22;
typedef StaMatrix<Real,2,2> MatR33;
typedef StaMatrix<Real,4,3> MatR43;
typedef StaMatrix<Real,4,3> MatR44;

#endif
