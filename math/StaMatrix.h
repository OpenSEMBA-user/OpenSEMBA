#ifndef STAMATRIX_H_
#define STAMATRIX_H_

#include "MathMatrix.h"
#include "DynMatrix.h"

// Static Matrix class. It can't change its size in run time.
template<class T,unsigned int NROWS,unsigned int NCOLS>
class StaMatrix : public MathMatrix<T> {
public:
   StaMatrix();
   StaMatrix(const StaMatrix<T,NROWS,NCOLS>& param);
   ~StaMatrix();
   T
    operator()(const unsigned int row, const unsigned int col) const;
   T&
    operator()(const unsigned int row, const unsigned int col);
   T
    val(const unsigned int ind) const;
   T&
    val(const unsigned int ind);
   T
    val(const unsigned int row, const unsigned int col) const;
   T&
    val(const unsigned int row, const unsigned int col);
   const T*
    val() const;
   unsigned int
    nRows() const;
   unsigned int
    nCols() const;
   StaMatrix<T,NROWS,NCOLS>&
    operator=(const StaMatrix<T,NROWS,NCOLS>& param);
   StaMatrix<T,NROWS,NCOLS>&
    operator=(const DynMatrix<int>& rhs);
   StaMatrix<T,NROWS,NCOLS>&
    assign(const StaMatrix<int,NROWS,NCOLS>& param);
   StaMatrix<T,NROWS,NCOLS>&
    operator=(const DynMatrix<double>& param);
   StaMatrix<T,NROWS,NCOLS>&
    operator+=(const double param);
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
    printInfo(unsigned int, unsigned int) const;
private:
   T _val[NROWS*NCOLS];
};
// =========== Not member functions ===========================================
// Products between different sized StaMatrices.
template<class T, class S, unsigned int NROWS, unsigned int NCOLS, unsigned int NCOLSB>
StaMatrix<T,NROWS,NCOLSB>
 operator*(const StaMatrix<T,NROWS,NCOLS>& lhs,
        const StaMatrix<S,NCOLS,NCOLSB>& rhs);
// Products between StaMatrix and DynMatrix
template<class T, unsigned int NROWS, unsigned int NCOLS>
DynMatrix<T>
 operator*(const StaMatrix<T,NROWS,NCOLS>& lhs, const DynMatrix<T>& rhs);

#include "StaMatrix.hpp"
#endif
