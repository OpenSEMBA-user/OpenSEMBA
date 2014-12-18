/* =============== MathMatrix procedures ==============================
   + Mathematical matrices class.
   + Contains two implementations:
     - DynMatrix (Dynamic matrices): Size can change in run time.
     - StaMatrix (Static matrices): Size set at compilation time.
==================================================================== */
#ifndef MATHMATRIX_H_
#define MATHMATRIX_H_

#ifndef _DEBUG
	#define NDEBUG
#endif
#ifndef MATRICES_ERROR
#	define MATRICES_ERROR 2
#endif
#define MATRICES_ASCENDING          0
#define MATRICES_DESCENDING         1
#ifndef MATRICES_MODES
#	define MATRICES_MODES
#	define MATRICES_COL_MAJOR	   0
#	define MATRICES_ROW_MAJOR      1
#endif

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <ctime>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <limits.h>
#include <assert.h>
#include "CartesianVector.h"
#ifdef USE_OPENMP
	#include <omp.h>
#endif

using namespace std;

template<class T>
class MathMatrix {
public:
	MathMatrix();
	virtual ~MathMatrix();
	virtual unsigned int
	 nCols() const = 0;
	virtual unsigned int
	 nRows() const = 0;
	virtual T
	 val(const unsigned int ind) const = 0;
	virtual T&
	 val(const unsigned int ind) = 0;
	virtual T
	 val(const unsigned int row, const unsigned int col) const = 0;
	virtual T&
	 val(const unsigned int row, const unsigned int col) = 0;
	virtual void
	 copy(vector<vector<T> > values) = 0;
	void
	 zeros();
	void
	 eye();
	void
	 cpLowerTri2UpperTri();
	void
	 convertToArray(const int mode, double *res) const;
	vector<CartesianVector<T,3> >
	 convertToCartesianVector() const;
	T
	 getDeterminant3x3() const;
	T
	 getDeterminant4x4() const;
	T
	 maxVal() const;
	T
	 maxValInCol(unsigned int col) const;
	virtual void
	 randomize(T min, T max);
	void
	 sortRows();
	void
	 sortRows(const unsigned int iCol, const unsigned int lCol);
	unsigned int
	 findFirstOcurrenceInColumns(
	  const T* vec,
	  const unsigned int col,
	  const unsigned int vecSize) const;
	bool
	 isSquare() const;
	bool
	 isSymmetric() const;
	vector<T>
	 cpRowToVector(const unsigned int row) const;
	virtual void
	 printInfo() const = 0;
protected:
	void
	 internalInvert();
	bool
	 isEQ(const T* x1, const T* x2, const unsigned int vS) const;
	bool
	 isGEQ(const T* x1, const T* x2, const unsigned int vS) const;
	bool
	 isLEQ(const T* x1, const T* x2, const unsigned int vS) const;
private:
	void
	 QSRows(long p, long r,
	  const unsigned int iCol, const unsigned int lCol);
	long
	 partitionRows(long p, long r,
	  const unsigned int iCol, const unsigned int lCol);
	unsigned int
	 binarySearch(
	  const T* key,
	  const unsigned int col,
	  const unsigned int vecSize,
	  unsigned int imin,
	  unsigned int imax) const;
	void
	 factorizeLU(int pivot[]);
	void
	 invertFactorized(const int pivot[]);
};
// =========== DynMatrix class ================================================
template<class T>
class DynMatrix : public MathMatrix<T> {
// Dynamic Matrix class. It can change it size in run time.
public:
	// ----------- ctors, dtors -----------------------------------------------
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
	bool
	 operator==(const DynMatrix<T>& param) const;
	bool
	 operator<(const DynMatrix<T>& param) const;
	DynMatrix<T>
	 convolute(const DynMatrix<T>& param) const;
	DynMatrix<T>&
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
// =========== StaMatrix class ================================================
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
// ============================================================================
#include "MathMatrix.hpp"
// ============================================================================
#endif
