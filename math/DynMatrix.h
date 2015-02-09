#ifndef DYNMATRIX_H_
#define DYNMATRIX_H_
#include "MathMatrix.h"
#include <utility>

template<class T>
class DynMatrix : public MathMatrix<T> {
public:
   DynMatrix();
   DynMatrix(const DynMatrix<T>& param);
   DynMatrix(unsigned int rows, unsigned int cols);
   DynMatrix(unsigned int rows, unsigned int cols, T** values);
   virtual ~DynMatrix();
   unsigned int
    nCols() const;
   unsigned int
    nRows() const;
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
    valPtr(const unsigned int row, const unsigned int col) const;
   // ----------- assignment, copy -------------------------------------------
   DynMatrix<T>&
    operator=(const DynMatrix<T> &param);
   DynMatrix<T>&
    operator=(const vector<T> &param);
   DynMatrix<T>&
    operator=(const T &param);
   void
    copy(
     vector<vector<T> > values);
   void
    copy(
     const DynMatrix<double> &param);
   DynMatrix<T>
    copy(
     const unsigned int fRow,
     const unsigned int lRow,
     const unsigned int fCol,
     const unsigned int lCol) const;
   // ----------- operations -------------------------------------------------
   DynMatrix<T>
    operator+(DynMatrix<T> &param) const;
   DynMatrix<T>&
    operator+=(const T param);
   DynMatrix<T>&
    operator+=(const DynMatrix& param);
   DynMatrix<T>&
    operator*=(const T param);
   DynMatrix<T>&
    operator/=(const T param);
   DynMatrix<T>
    operator*(DynMatrix<T> &param) const;
   DynMatrix<T>
    operator*(const T param) const;
   DynMatrix<T>
    operator/(const T param) const;
   DynMatrix<T>
    kron(DynMatrix<T>& param) const;
   DynMatrix<T>
    sub(pair<int,int>& rows, pair<int,int>& cols) const;
   bool
    operator==(const DynMatrix<T>& param) const;
   bool
    operator<(const DynMatrix<T>& param) const;
   DynMatrix<T>
    convolute(const DynMatrix<T>& param) const;
   DynMatrix<T>
    invert();
   DynMatrix<T>&
    transpose();
   DynMatrix<T>
    reshape(unsigned int rows, unsigned int cols);
   DynMatrix<T>
    reshape(unsigned int rows, unsigned int cols, int order);
   DynMatrix<T>
    eliminateColumns(unsigned int first, unsigned int last) const;
   DynMatrix<T>&
    sortAndRemoveRepeatedRows_omp();
   void
    sortAndRemoveRepeatedRows();
   void
    removeRepeatedSortedRows();
   void
    sortRows_omp(const unsigned int iCol, const unsigned int lCol);
   void
    mergeSortedRows(
     const DynMatrix<T>& a,
     const unsigned int iCol,
     const unsigned int lCol);
   void
    printInfo() const;
   void
    printInfo(unsigned int, unsigned int) const;
   void
    printInfo(
     unsigned int firstRow,
     unsigned int lastRow,
     unsigned int firstCol,
     unsigned int lastCol) const;
private:
   T * _val;
   unsigned int _nRows, _nCols;
   void
    resizeVal(const unsigned int rows, const unsigned int cols);
};

#include "DynMatrix.hpp"

#endif
