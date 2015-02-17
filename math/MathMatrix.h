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

#include "MathMatrix.hpp"

#endif
