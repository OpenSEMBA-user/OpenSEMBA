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

#define MATRICES_ASCENDING          0
#define MATRICES_DESCENDING         1
#ifndef MATRICES_MODES
#	define MATRICES_MODES
#	define MATRICES_COL_MAJOR	   0
#	define MATRICES_ROW_MAJOR      1
#endif

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#include "CartesianVector.h"
#ifdef USE_OPENMP
	#include <omp.h>
#endif

template<class T>
class MathMatrix {
public:
	MathMatrix();
	virtual ~MathMatrix();
	virtual UInt
	 nCols() const = 0;
	virtual UInt
	 nRows() const = 0;
	virtual T
	 val(const UInt ind) const = 0;
	virtual T&
	 val(const UInt ind) = 0;
	virtual T
	 val(const UInt row, const UInt col) const = 0;
	virtual T&
	 val(const UInt row, const UInt col) = 0;
	virtual void
	 copy(vector<vector<T> > values) = 0;
	void
	 zeros();
	void
	 eye();
	void
	 cpLowerTri2UpperTri();
	void
	 convertToArray(const Int mode, Real *res) const;
	vector<CartesianVector<T,3> >
	 convertToCartesianVector() const;
	T
	 getDeterminant3x3() const;
	T
	 getDeterminant4x4() const;
	T
	 maxVal() const;
	T
	 maxValInCol(UInt col) const;
	virtual void
	 randomize(T min, T max);
	void
	 sortRows();
	void
	 sortRows(const UInt iCol, const UInt lCol);
	UInt
	 findFirstOcurrenceInColumns(
	  const T* vec,
	  const UInt col,
	  const UInt vecSize) const;
	bool
	 isSquare() const;
	bool
	 isSymmetric() const;
	vector<T>
	 cpRowToVector(const UInt row) const;
	virtual void
	 printInfo() const = 0;
protected:
	void
	 internalInvert();
	bool
	 isEQ(const T* x1, const T* x2, const UInt vS) const;
	bool
	 isGEQ(const T* x1, const T* x2, const UInt vS) const;
	bool
	 isLEQ(const T* x1, const T* x2, const UInt vS) const;
private:
	void
	 QSRows(Int p, Int r,
	  const UInt iCol, const UInt lCol);
	Int
	 partitionRows(Int p, Int r,
	  const UInt iCol, const UInt lCol);
	UInt
	 binarySearch(
	  const T* key,
	  const UInt col,
	  const UInt vecSize,
	  UInt imin,
	  UInt imax) const;
	void
	 factorizeLU(Int pivot[]);
	void
	 invertFactorized(const Int pivot[]);
};

#include "MathMatrix.hpp"

#endif
