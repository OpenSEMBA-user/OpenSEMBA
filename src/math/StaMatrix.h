#ifndef STAMATRIX_H_
#define STAMATRIX_H_

#include "MathMatrix.h"
#include "DynMatrix.h"

// Static Matrix class. It can't change its size in run time.
template<class T,UInt ROWS,UInt COLS>
class StaMatrix : public MathMatrix<T> {
public:
   StaMatrix();
   StaMatrix(const StaMatrix<T,ROWS,COLS>& rhs);
   ~StaMatrix();
   T operator()(const UInt row, const UInt col) const;
   T& operator()(const UInt row, const UInt col);
   T val(const UInt ind) const;
   T& val(const UInt ind);
   T val(const UInt row, const UInt col) const;
   T& val(const UInt row, const UInt col);
   const T* val() const;
   UInt nRows() const;
   UInt nCols() const;

   StaMatrix<T,ROWS,COLS>& operator=(const StaMatrix<T,ROWS,COLS>& rhs);
   StaMatrix<T,ROWS,COLS>& operator=(const DynMatrix<Int>& rhs);
   StaMatrix<T,ROWS,COLS>& operator=(const DynMatrix<Real>& rhs);

   StaMatrix<T,ROWS,COLS>& assign(const StaMatrix<Int,ROWS,COLS>& rhs);
   StaMatrix<T,ROWS,COLS>& setInDiagonal(const CartesianVector<T,ROWS>& rhs);

   StaMatrix<T,ROWS,COLS>& operator+=(const Real rhs);
   StaMatrix<T,ROWS,COLS>& operator+=(const StaMatrix<T,ROWS,COLS>& rhs);
   StaMatrix<T,ROWS,COLS>& operator-=(const StaMatrix<T,ROWS,COLS>& rhs);
   StaMatrix<T,ROWS,COLS>& operator*=(const T rhs);
   StaMatrix<T,ROWS,COLS>& operator/=(const T rhs);
   StaMatrix<T,ROWS,COLS> operator*(T rhs) const;
   CartesianVector<T,COLS> operator*(CartesianVector<T,COLS> rhs) const;
   StaMatrix<T,ROWS,COLS> operator+(StaMatrix<T,ROWS,COLS> &rhs) const;
   StaMatrix<T,COLS,ROWS>& invert();
   StaMatrix<T,COLS,ROWS> transpose();

   void copy(vector<vector<T> > values);
   bool operator==(const StaMatrix<T,ROWS,COLS>& rhs) const;
   bool operator<(const StaMatrix<T,ROWS,COLS>& rhs) const;
   void printInfo() const;
   void printInfo(UInt, UInt) const;
private:
   T _val[ROWS*COLS];
};

// Products between different sized StaMatrices.
template<class T, class S, UInt ROWS, UInt COLS, UInt NCOLSB>
StaMatrix<T,ROWS,NCOLSB> operator*(
        const StaMatrix<T,ROWS,COLS>& lhs,
        const StaMatrix<S,COLS,NCOLSB>& rhs);
// Products between StaMatrix and DynMatrix
template<class T, UInt ROWS, UInt COLS>
DynMatrix<T> operator*(
        const StaMatrix<T,ROWS,COLS>& lhs,
        const DynMatrix<T>& rhs);

#include "StaMatrix.hpp"

typedef StaMatrix<Real,2,2> MatR22;
typedef StaMatrix<Real,3,3> MatR33;
typedef StaMatrix<Real,4,3> MatR43;
typedef StaMatrix<Real,4,3> MatR44;

#endif
