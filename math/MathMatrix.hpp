// File: MathMatrix.cpp
// =============== Includes headers ===========================================
#ifndef MATHMATRIX_H_
	#include "MathMatrix.h"
#endif

// =============== MathMatrix<T> methods ======================================
template <class T>
MathMatrix<T>::MathMatrix() {

}

template <class T>
MathMatrix<T>::~MathMatrix() {

}

template <class T>
T
MathMatrix<T>::getDeterminant3x3() const {
	assert(nRows() == 3 && nCols() == 3);
	return	+ val(0,0) * (val(1,1)*val(2,2) - val(2,1)*val(1,2))
			- val(0,1) * (val(1,0)*val(2,2) - val(1,2)*val(2,0))
            + val(0,2) * (val(1,0)*val(2,1) - val(1,1)*val(2,0));
}
 
template <class T>
T
MathMatrix<T>::getDeterminant4x4() const {
	assert(nRows() == 4 && nCols() == 4);
	return val(0,3) * val(1,2) * val(2,1) * val(3,0)
	     - val(0,2) * val(1,3) * val(2,1) * val(3,0)
 		 - val(0,3) * val(1,1) * val(2,2) * val(3,0)
		 + val(0,1) * val(1,3) * val(2,2) * val(3,0)
		 + val(0,2) * val(1,1) * val(2,3) * val(3,0)
		 - val(0,1) * val(1,2) * val(2,3) * val(3,0)
		 - val(0,3) * val(1,2) * val(2,0) * val(3,1)
		 + val(0,2) * val(1,3) * val(2,0) * val(3,1)
		 + val(0,3) * val(1,0) * val(2,2) * val(3,1)
		 - val(0,0) * val(1,3) * val(2,2) * val(3,1)
		 - val(0,2) * val(1,0) * val(2,3) * val(3,1)
		 + val(0,0) * val(1,2) * val(2,3) * val(3,1)
		 + val(0,3) * val(1,1) * val(2,0) * val(3,2)
		 - val(0,1) * val(1,3) * val(2,0) * val(3,2)
		 - val(0,3) * val(1,0) * val(2,1) * val(3,2)
		 + val(0,0) * val(1,3) * val(2,1) * val(3,2)
		 + val(0,1) * val(1,0) * val(2,3) * val(3,2)
		 - val(0,0) * val(1,1) * val(2,3) * val(3,2)
		 - val(0,2) * val(1,1) * val(2,0) * val(3,3)
		 + val(0,1) * val(1,2) * val(2,0) * val(3,3)
		 + val(0,2) * val(1,0) * val(2,1) * val(3,3)
		 - val(0,0) * val(1,2) * val(2,1) * val(3,3)
		 - val(0,1) * val(1,0) * val(2,2) * val(3,3)
		 + val(0,0) * val(1,1) * val(2,2) * val(3,3);
}

 
template <class T>
void
MathMatrix<T>::internalInvert() {
	// TODO Include static_assert to avoid not supported type inversions.
	assert(MathMatrix<T>::isSquare());
	int *pivot = new int[nRows()];
	factorizeLU(pivot);
	invertFactorized(pivot);
	delete [] pivot;
}
 
template<class T>
void
MathMatrix<T>::factorizeLU(int pivot[]) {
	// Performs a LINPACK-style PLU factorization of a general matrix.
	// Output, unsigned int PIVOT[N], a vector of pivot indices.
	assert(nRows() == nCols());
	unsigned int n = nRows();
	unsigned int i, j, k, l;
	T t;
	for (k = 0; k < n-1; k++) {
		//  Find L, the index of the pivot row.
		l = k;
		for (i = k; i < n; i++) {
			if (abs(val(l,k)) < abs(val(i,k))) {
				l = i;
			}
		}
		pivot[k] = l;
		//  If the pivot index is zero, the algorithm has failed.
		if (val(l,k) == (T) 0.0)	{
			cout << "ERROR@MathMatrix<T>::factorizeLU" << endl;
			cout << "  Zero pivot on step " << k << endl;
			printInfo();
			assert(false);
			exit (MATRICES_ERROR);
		}
		//  Interchange rows L and K if necessary.
		if (l != k) {
			t = val(l,k);
			val(l,k) = val(k,k);
			val(k,k) = t;
		}
		//  Normalize the values that lie below the pivot entry A(K,K).
		for (i = k+1; i < n; i++) {
			val(i,k) = - val(i,k) / val(k,k);
		}
		//  Row elimination with column indexing.
		for (j = k+1; j < n; j++) {
			if (l != k)	{
				t = val(l,j);
				val(l,j) = val(k,j);
				val(k,j) = t;
			}
			for (i = k+1; i < n; i++) {
				val(i,j) += val(i,k) * val(k,j);
			}
		}
	}
	pivot[n-1] = n-1;
	if (val(n-1,n-1) == (T) 0.0) {
		cout << "ERROR@MathMatrix<T>::factorizeToArray" << endl;
		cout << "  Zero pivot on step " << n-1 << endl;
		printInfo();
		assert(false);
		exit (MATRICES_ERROR);
	}
}

template <class T>
void
MathMatrix<T>::invertFactorized(const int pivot[]) {
	// Computes inverse of a general matrix factored by factorize.
	// Input, int PIVOT(N), the pivot vector from R8GE_FA.
	assert(nRows() == nCols());
	int n = nRows();
	int nn = n * n;
	int i, j, k;
	T temp;
	double *b = new double[nn];
	// Compute Inverse(U).
	for (k = 0; k < n; k++) {
		for (i = 0; i < k; i++) {
			b[i+k*n] = -b[i+k*n] / val(k,k);
	    }
	    b[k+k*n] = 1.0 / val(k,k);
	    for (j = k+1; j < n; j++) {
	    	b[k+j*n] = (T) 0.0;
	    	for (i = 0; i <= k; i++) {
	    		b[i+j*n] = b[i+j*n] + b[i+k*n] * val(k,j);
	    	}
	    }
	}

	// Multiply Inverse(U) by Inverse(L).
	for (k = n-2; k >= 0; k--) {
	    for (i = k+1; i < n; i++) {
	    	b[i+k*n] = 0.0;
	    }
	    for (j = k+1; j < n; j++) {
	    	for (i = 0; i < n; i++) {
	    		b[i+k*n] = b[i+k*n] + b[i+j*n] * val(j,k);
	    	}
	    }
	    if (pivot[k] != k) {
	    	for (i = 0; i < n; i++) {
	    		temp = b[i+k*n];
	    		b[i+k*n] = b[i+pivot[k]*n];
	    		b[i+pivot[k]*n] = temp;
	    	}
	    }
	}
	// Stores inverted matrix in res.
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			val(i,j) = (T) b[i+j*n];
		}
	}
	// Frees memory.
	delete [] b;
}
 
template <class T>
T
MathMatrix<T>::maxVal() const {
	T res = val(0,0);
	for (unsigned int i = 0; i < nRows(); i++) {
		for (unsigned int j = 0; j < nCols(); j++) {
			if (val(i,j) > res)
				res = val(i,j);
		}
	}
	return res;
}
 
template <class T>
T
MathMatrix<T>::maxValInCol(unsigned int col) const {
	T res = val(0,col);
	for (unsigned int i = 0; i < nRows(); i++)
		if (val(i,col) > res)
			res = val(i,col);
	return res;
}

template <class T>
void
MathMatrix<T>::randomize(T min, T max) {
	double range = (double) max - (double) min;
	for (unsigned int i = 0; i < nRows(); i++) {
		for (unsigned int j = 0; j < nCols(); j++) {
			double aux = range * rand() / (RAND_MAX + (double) 1) + min;
			val(i,j) = (T) aux;
		}
	}
}

template <class T>
void
MathMatrix<T>::sortRows() {
	// Orders array a with nr rows and nc columns. Ordering is performed
	// using column indicated in orCol as reference. In ascending order.
	sortRows(0, nCols());
}
 
template <class T>
void
MathMatrix<T>::sortRows(const unsigned int iCol, const unsigned int lCol) {
	// Orders array a with nr rows and nc columns. Ordering is performed
	// using column indicated in orCol as reference. In ascending order.
	if (nRows() <= 1) {
		return;
	}
	QSRows(0, nRows()-1, iCol, lCol);
}

template <class T>
bool
MathMatrix<T>::isEQ(
 const T* x1,
 const T* x2,
 const unsigned int vS) const {
	for (unsigned int i = 0; i < vS; i++) {
		if (x1[i] != x2[i]) {
			return false;
		}
	}
	return true;
}

template <class T>
bool
MathMatrix<T>::isGEQ(
 const T* x1,
 const T* x2,
 const unsigned int vS) const {
	for (unsigned int i = 0; i < vS; i++) {
		if (x1[i] < x2[i]) {
			return false;
		}
		if (x1[i] > x2[i]) {
			return true;
		}
	}
	return true;
}

template <class T>
bool
MathMatrix<T>::isLEQ(
 const T* x1,
 const T* x2,
 const unsigned int vS) const {
	for (unsigned int i = 0; i < vS; i++) {
		if (x1[i] > x2[i]) {
			return false;
		}
		if (x1[i] < x2[i]) {
			return true;
		}
	}
	return true;
}

template <class T>
long
MathMatrix<T>::partitionRows(
 long p, long r,
 const unsigned int iCol, const unsigned int lCol) {
	long j = p - 1;
	for (long i = p; i < r; i++) {
		bool geq = isGEQ(&val(r,iCol), &val(i,iCol), lCol - iCol + 1);
		if (geq) {
			j++;
			for (unsigned int k = 0; k < nCols(); k++) {
				T temp = val(j,k);
				val(j,k) = val(i,k);
				val(i,k) = temp;
			}
		}
	}
	for (unsigned int k = 0; k < nCols(); k++) {
		T x = val(r, k);
		val(r, k) = val(j+1, k);
		val(j+1, k) = x;
	}
	return (j + 1);
}

template <class T>
void
MathMatrix<T>::QSRows(
 long p, long r, const unsigned int iCol, const unsigned int lCol) {
	if (p < r) {
		long q = partitionRows(p, r, iCol, lCol);
		QSRows(p, q - 1, iCol, lCol);
		QSRows(q + 1, r, iCol, lCol);
	}
}

template <class T>
unsigned int
MathMatrix<T>::binarySearch(
 const T* key,
 const unsigned int col,
 const unsigned int vecSize,
 unsigned int imin,
 unsigned int imax) const {
	// NOTE: Just for ascending order!!!
	if (imax == imin) {
		if (nRows() == imin) {
			return imin;
		}
		// Checks that this value corresponds to the value searched.
		T *value;
		value = new T [vecSize];

		for (unsigned int i = 0; i < vecSize; i++) {
			value[i] = val(imin, col+i);
		}
		bool iseq;
		iseq = isEQ(value, key, vecSize);
		delete[] value;
		if (iseq) {			
			return imin;
		} else {
			return nRows();
		}
	} else {
		unsigned int imid = (unsigned int) (imin + imax) / 2;
		T *value;
		value = new T [vecSize];
		for (unsigned int i = 0; i < vecSize; i++) {
			value[i] = val(imid, col+i);
		}
		bool geq = isGEQ(value, key, vecSize);
		delete[] value;
		if (geq) {
			return binarySearch(key, col, vecSize, imin, imid);
		} else {
			return binarySearch(key, col, vecSize, imid+1, imax);
		}
    }
}
 
template <class T>
unsigned int
MathMatrix<T>::findFirstOcurrenceInColumns(
 const T* key,
 const unsigned int col,
 const unsigned int vecSize) const {
	// Performs binary search -------------------------------------------------
	unsigned int row = binarySearch(key, col, vecSize, 0, nRows());
	if (row == nRows()) {
		return row; // Returns this is value was not found.
	}
	// Goes back to find the first occurrence ---------------------------------
	for (unsigned int i = row; i > 0; i--) {
		bool isEqual = true;
		for (unsigned int j = 0; j < vecSize; j++) {
			isEqual &= val(i,j+col) == key[j];
		}
		if (!isEqual) {
			return i+1;
		}
	}
	// Does a final check that the row corresponds to the value.

	// Returns result.
	return row;
}
 
template <class T>
void
MathMatrix<T>::cpLowerTri2UpperTri() {
	unsigned int i, j;
	for (i = 0; i < nRows(); i++)
		for (j = i+1; j < nCols(); j++)
			val(i,j) = val(j,i);
}
 
template <class T>
void
MathMatrix<T>::zeros() {
	for (unsigned int i = 0; i < nRows(); i++)
		for (unsigned int j = 0; j < nCols(); j++)
 			val(i,j) = (T) 0;
}
 
template <class T>
void
MathMatrix<T>::convertToArray(const int mode, double *res) const {
	if (mode == MATRICES_COL_MAJOR) {
		for (unsigned int j = 0; j < nCols(); j++)
			for (unsigned int i = 0; i < nRows(); i++)
				res[i + j * nRows()] = val(i,j);
		return;
	} else if (mode == MATRICES_ROW_MAJOR) {
		for (unsigned int i = 0; i < nRows(); i++)
			for (unsigned int j = 0; j < nCols(); j++)
				res[i * nCols() + j] = val(i,j);
		return;
	} else {
		cerr << "ERROR: DynMatrix<T>::converToArray "     << endl;
		cerr << "Inserted mode is not defined."           << endl;
		cerr << "Terminating."                            << endl;
		exit(MATRICES_ERROR);
	}
}
 
template <class T>
bool
MathMatrix<T>::isSquare() const {
	return (nRows() == nCols());
}

template <class T>
bool
MathMatrix<T>::isSymmetric() const {
	for (unsigned int i = 0; i < nRows(); i++) {
		for (unsigned int j = 0; j < nRows(); j++) {
			if (val(i,j) != val(j,i)) {
				return false;
			}
		}
	}
	return true;
}
 
template <class T>
void
MathMatrix<T>::eye() {
	assert(isSquare());
	// Sets 1 in main diagonal or 0 otherwise.
	zeros();
	for (unsigned int i = 0; i < nRows(); i++)
		val(i,i) = T(1);
}
 
template <class T>
vector<T>
MathMatrix<T>::cpRowToVector(const unsigned int row) const {
	vector<T> res(nCols());
	for (unsigned int i = 0; i < nCols(); i++) {
		res[i] = val(row,i);
	}
	return res;
}

template <class T>
vector<CartesianVector<T,3> >
MathMatrix<T>::convertToCartesianVector() const {
	assert(nCols() == 3);
	vector<CartesianVector<T,3> > res(nRows());
	for (uint r = 0; r < nRows(); r++) {
		res[r] = CartesianVector<T,3>(val(r,0), val(r,1), val(r,2));
	}
	return res;
}


template <class T>
DynMatrix<T>::DynMatrix() {
	_val = NULL;
	_nRows = 0;
	_nCols = 0;
	for (unsigned int i = 0; i < _nRows*_nCols; i++) {
		_val[i] = (T) 0.0;
	}
}

template <class T>
DynMatrix<T>::DynMatrix(const DynMatrix<T>& param) {
	_nRows = param._nRows;
	_nCols = param._nCols;
	unsigned int nRnC = _nRows * _nCols;
	_val = new T[nRnC];
	for (unsigned int i = 0; i < nRnC; i++) {
		_val[i] = (T) param._val[i];
	}
}
 
template <class T>
DynMatrix<T>::DynMatrix(unsigned int rows, unsigned int cols) {
	_nRows = rows;
	_nCols = cols;
	_val = new T[_nRows * _nCols];
	for (unsigned int i = 0; i < _nRows*_nCols; i++) {
		_val[i] = (T) 0.0;
	}
}

template <class T>
DynMatrix<T>::DynMatrix(unsigned int rows, unsigned int cols, T** values) {
	_nRows = rows;
	_nCols = cols;
	assert(values != NULL);
	for (unsigned int i = 0; i < _nRows; i++) {
		for (unsigned int j = 0; j < _nCols; j++) {
			val(i,j) = values[i][j];
		}
	}
}

template <class T>
DynMatrix<T>::~DynMatrix<T>() {
	if (_val != NULL) {
		delete [] _val;
	}
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::operator=(const DynMatrix<T>& param) {
	if (this == &param) {
		return *this;
	}
	_nRows = param._nRows;
	_nCols = param._nCols;
	resizeVal(_nRows, _nCols);
	unsigned int nRnC = _nRows * _nCols;
	for (unsigned int i = 0; i < nRnC; i++) {
		_val[i] = param._val[i];
	}
	return *this;
}

template<class T>
inline unsigned int
DynMatrix<T>::nCols() const {
	return _nCols;
}

template<class T>
inline unsigned int
DynMatrix<T>::nRows() const {
	return _nRows;
}
 
template <class T>
inline void
DynMatrix<T>::resizeVal(const unsigned int rows, const unsigned int cols) {
	_nRows = rows;
	_nCols = cols;
	if (_val != NULL) {
		delete [] _val;
	}
	_val = new T[_nRows*_nCols];
}
 
template<class T>
void
DynMatrix<T>::copy(vector<vector<T> > values) {
	unsigned int rows = values.size();
	unsigned int cols;
	if (rows > 0) {
		cols = values[0].size();
	} else {
		cols = 0;
	}
	resizeVal(rows, cols);
	for (unsigned int i = 0; i < _nRows; i++) {
		for (unsigned int j = 0; j < _nCols; j++) {
			val(i,j) = values[i][j];
		}
	}
}
 
template <class T>
DynMatrix<T>&
DynMatrix<T>::operator=(const vector<T>& param) {
	resizeVal(param.size(), 1);
	for (unsigned int i = 0; i < _nRows; i++) {
		val(i,0) = param[i];
	}
	return *this;
}
 
template <class T>
void
DynMatrix<T>::copy(const DynMatrix<double>& param) {
	_nRows = param.nRows();
	_nCols = param.nCols();
	resizeVal(nRows(), nCols());
	for (unsigned int r = 0; r < _nRows; r++) {
		for (unsigned int c = 0; c < _nCols; c++) {
			val(r,c) = param(r,c);
		}
	}
}

template<class T>
DynMatrix<T>
DynMatrix<T>::copy(
 const unsigned int fRow,
 const unsigned int lRow,
 const unsigned int fCol,
 const unsigned int lCol) const {
	unsigned int rS = lRow - fRow;
	unsigned int cS = lCol - fCol;
	DynMatrix<T> res(rS, cS);
	for (unsigned int i = 0; i < rS; i++) {
		for (unsigned int j = 0; j < cS; j++) {
			res(i,j) = val(i+fRow, j+fCol);
		}
	}
	return res;
}

template<class T>
DynMatrix<T>&
DynMatrix<T>::sortAndRemoveRepeatedRows_omp() {
	sortRows_omp(0, nCols());
	removeRepeatedSortedRows();
	return *this;
}

template<class T>
void
DynMatrix<T>::sortAndRemoveRepeatedRows() {
	this->sortRows(0, nCols());
	removeRepeatedSortedRows();
}

template<class T>
void
DynMatrix<T>::removeRepeatedSortedRows() {
	vector<vector<T> > aux;
	aux.reserve(nRows());
	unsigned int nonRepeated = 0;
	unsigned int nCompared;
	if (nRows() <= 1 ) {
		nCompared = 0;
	} else {
		nCompared = nRows() - 1;
	}
	for (unsigned int i = 0; i < nCompared; i++) {
		bool areEqual = true;
		for (unsigned int j = 0; j < nCols(); j++) {
			areEqual &= (val(i,j) == val(i+1,j));
		}
		if (!areEqual) {
			nonRepeated++;
			aux.push_back(this->cpRowToVector(i));
			if (i == nRows() - 2) {
				nonRepeated++;
				aux.push_back(this->cpRowToVector(i+1));
			}
		}
		if ((i == nRows() - 2) && areEqual) {
			nonRepeated++;
			aux.push_back(this->cpRowToVector(i+1));
		}
	}
	_nRows = nonRepeated;
	copy(aux);
}


template<class T>
void
DynMatrix<T>::sortRows_omp(
 const unsigned int iCol, const unsigned int lCol) {
#ifdef USE_OPENMP
	unsigned int nT = (unsigned int) omp_get_max_threads();
	if (_nRows < nT || nT == 1) {
		this->sortRows(iCol, lCol);
		return;
	}
	// Each thread copies and sorts its respective work chunk.
	DynMatrix<T> chunk[nT];
	unsigned int cSize = nRows() / nT;
	unsigned int tId;
	#pragma omp parallel private (tId) shared(chunk)
	{
		tId = omp_get_thread_num();
		unsigned int fRow = tId * cSize;
		unsigned int lRow = (tId + 1) * cSize;
		if (tId + 1 == nT) {
			lRow = nRows();
		}
		chunk[tId] = copy(fRow,lRow,0,nCols());
		chunk[tId].sortRows(iCol, lCol);
	}
	// Combines all the chunks.
	*this = chunk[0];
	for (unsigned int i = 1; i < nT; i++) {
		mergeSortedRows(chunk[i], iCol, lCol);
	}
#else
	this->sortRows(iCol, lCol);
#endif
}

 
template <class T>
DynMatrix<T>&
DynMatrix<T>::operator=(const T &param) {
	for (unsigned int i = 0; i < _nRows; i++)
		for (unsigned int j = 0; j < _nCols; j++)
			_val[i][j] = param;
	return *this;
}
 
template <class T>
inline T
DynMatrix<T>::operator() (const unsigned int row, const unsigned int col) const {
	assert(row < _nRows && col < _nCols);
	return _val[row * _nCols + col];
}
 
template <class T>
inline T&
DynMatrix<T>::operator()(const unsigned int row, const unsigned int col) {
	assert(col < _nCols);
	assert(row < _nRows);
	return _val[row * _nCols + col];
}
 
template <class T>
inline T
DynMatrix<T>::val(const unsigned int i) const {
	assert(i < _nRows * _nCols);
	return _val[i];
}
 
template <class T>
inline T&
DynMatrix<T>::val(const unsigned int i) {
	assert(i < _nRows * _nCols);
	return _val[i];
}
 
template <class T>
inline T
DynMatrix<T>::val(const unsigned int row, const unsigned int col) const {
	assert(col < _nCols);
	assert(row < _nRows);
	return _val[row * _nCols + col];
}
 
template <class T>
inline T&
DynMatrix<T>::val(const unsigned int row, const unsigned int col) {
	assert(col < _nCols);
	assert(row < _nRows);
	return _val[row * _nCols + col];
}

template <class T>
const T*
DynMatrix<T>::valPtr(const unsigned int row, const unsigned int col) const {
	assert(row < _nRows && col < _nCols);
	return &_val[row * _nCols + col];
}
 
template <class T>
DynMatrix<T>
DynMatrix<T>::operator*(DynMatrix<T> &param) const {
	DynMatrix<T> res(_nRows, param._nCols);
	unsigned int i, j, k;
	for (i = 0; i < _nRows; i++) {
		for (k = 0; k < _nCols; k++) {
			for (j = 0; j < param._nCols; j++) {
				res(i,j) += val(i,k) * param(k,j);
			}
		}
	}
	return res;
}
 
template <class T>
DynMatrix<T>
DynMatrix<T>::operator+(DynMatrix<T> &param) const {
	assert(_nRows == param._nRows && _nCols == param._nCols);
	DynMatrix<T> res(_nRows, _nCols);
	unsigned int n = _nRows * _nCols;
	for (unsigned int i = 0; i < n; i++) {
		res._val[i] = _val[i] + param._val[i];
	}
	return res;
}
 
template <class T>
DynMatrix<T>
DynMatrix<T>::operator*(const T param) const {
	unsigned int i, j;
	DynMatrix<T> res(_nRows, _nCols);
	for (i = 0; i < _nRows; i++) {
		for (j = 0; j < _nCols; j++) {
			res._val[i][j] = _val[i][j] * param;
		}
	}
	return res;
}
 
template<class T>
DynMatrix<T>
DynMatrix<T>::eliminateColumns(
 unsigned int first, unsigned int last) const {
	unsigned int nColsToEliminate = last - first + 1;
	DynMatrix<T> res(_nRows, _nCols - nColsToEliminate);
	for (unsigned int r = 0; r < _nRows; r++) {
		for (unsigned int c = 0; c < first; c++) {
			res(r,c) = val(r,c);
		}
	}
	for (unsigned int r = 0; r < _nRows; r++) {
		for (unsigned int c = last+1; c < _nCols; c++) {
			res(r,c-nColsToEliminate) = val(r,c);
		}
	}
	return res;
}
 
template <class T>
DynMatrix<T>
DynMatrix<T>::operator/(const T param) const {
	unsigned int i,j;
	DynMatrix<T> res(_nRows, _nCols);
	for (i = 0; i < _nRows; i++)
		for (j = 0; j < _nCols; j++)
			res._val[i][j] /= param;
	return res;
}
 
template <class T>
DynMatrix<T>&
DynMatrix<T>::operator+=(const T param) {
	unsigned int n = _nRows * _nCols;
	for (unsigned int i = 0; i < n; i++)
		_val[i] += param;
	return *this;
}
 
template <class T>
DynMatrix<T>&
DynMatrix<T>::operator+=(const DynMatrix<T>& param) {
	assert(_nRows == param._nRows && _nCols == param._nCols);
	unsigned int n = _nRows * _nCols;
	for (unsigned int i = 0; i < n; i++)
		_val[i] += param._val[i];
	return *this;
}
 
template <class T>
DynMatrix<T>&
DynMatrix<T>::operator*=(const T param) {
	assert(_nRows == param._nRows && _nCols == param._nCols);
	unsigned int i, j;
	for (i = 0; i < _nRows; i++) {
		for (j = 0; j < _nCols; j++) {
			_val[i][j] *= param;
		}
	}
	return *this;
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::operator/=(const T param) {
	assert(_nRows == param._nRows && _nCols == param._nCols);
	unsigned int i, j;
	for (i = 0; i < _nRows; i++) {
		for (j = 0; j < _nCols; j++) {
			_val[i][j] /= param;
		}
	}
	return *this;
}
 

template <class T>
bool
DynMatrix<T>::operator==(const DynMatrix<T>& param) const {
	bool res = true;
	if (_nRows == param._nRows && _nCols == param._nCols) {
		for (unsigned int i = 0; i < _nRows; i++)
			for (unsigned int j = 0; j < _nCols; j++)
				res &= (_val[i][j] == param._val[i][j]);
		return res;
	} else
		return false;
}
 
template <class T>
bool
DynMatrix<T>::operator<(const DynMatrix<T>& param) const {
	assert(_nRows == param._nRows && _nCols == param._nCols);
	for (unsigned int i = 0; i < _nRows; i++)
		for (unsigned int j = 0; j < _nCols; j++)
			if (_val[i][j] < param._val[i][j])
				return true;
	return false;
}
 
template <class T>
DynMatrix<T>
DynMatrix<T>::convolute(const DynMatrix<T> &param) const {
	// PURPOSE:
	// Returns convolution of this row matrix vector and param.
	assert(_nCols == 1 && param._nCols == 1);
	unsigned  int i, j, min, max;
	DynMatrix<T> res(_nRows + param._nRows - 1, 1);
	min = _nRows;
	max = 1;
	for (j = 1; j <= _nRows + param._nRows - 1; j++) {
		if (j <= _nRows)
			min = j;
		if (j + 1 - param._nRows >= 1)
			max = j + 1 - param._nRows;
		for (i = max; i <= min; i++)
			res(j-1,0) += val(i-1,0) * param(j-i,0);
	}
	return res;
}
 
template <class T>
void
DynMatrix<T>::printInfo() const {
	printInfo(_nRows, _nCols);
}
 
template <class T>
void
DynMatrix<T>::printInfo(unsigned int rows, unsigned int cols) const {
	unsigned int i, j;
	if (rows > _nRows || cols > _nCols) {
		cerr << "ERROR: Unable print more rows or cols than available" << endl;
		cerr << "Terminating."                                         << endl;
		exit(MATRICES_ERROR);
	}
	cout << "Dimensions: " << _nRows << "x" << _nCols << endl;
	cout << "Stored values: " << endl;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++)
			cout << setprecision(16) << val(i,j) << " ";
		cout << endl;
	}
}
 
template <class T>
void
DynMatrix<T>::printInfo(
 unsigned int fr, unsigned int lr,
 unsigned int fc, unsigned int lc) const {
	cout << "Dimensions: " << _nRows << "x" << _nCols << endl;
	cout << "Stored values: " << endl;
	for (unsigned int i = fr; i <= lr; i++) {
		for (unsigned int j = fc; j < lc; j++)
			cout << _val[i][j] << " ";
		cout << endl;
	}
}
 
template <class T>
DynMatrix<T>
DynMatrix<T>::reshape(unsigned int rows, unsigned int cols) {
	DynMatrix<T> res(rows, cols);
	res = reshape(rows,cols,1);
	return res;
}
 
template <class T>
DynMatrix<T>
DynMatrix<T>::reshape(unsigned int rows, unsigned int cols, int order) {
	assert(_nRows * _nCols == rows*cols);
	DynMatrix<T> res(rows, cols);
	unsigned int i, j, k;
	k = 0;
	switch (order) {
	case 1:
		// Performs ordering taking elements from one row at a time.
		for (i = 0; i < rows; i++)
			for (j = 0; j < cols; j++) { 
				res._val[i][j] = _val[k / _nCols][k % _nCols];
				k++;
			}
		break;
	case 2:
		// Performs ordering taking elements from one column at a time.
		for (i = 0; i < rows; i++)
			for (j = 0; j < cols; j++) {
				res._val[i][j] = _val[k % _nRows][k / _nRows];
				k++;
			}
		break;
	}
	return res;
}
 
template<class T>
DynMatrix<T>
DynMatrix<T>::kron(DynMatrix<T>& rhs) const {
	DynMatrix<T> res(_nRows + rhs._nRows, _nCols + rhs._nCols);
	for (unsigned int i = 0; i < _nRows; i++) {
		for (unsigned int j = 0; j < rhs._nRows; j++) {
			for (unsigned int k = 0; k < _nCols; k++) {
				for (unsigned int l = 0; l < rhs._nCols; l++) {
					res(i*_nRows + j, k*_nCols + l) = val(i,k) * rhs(j,l);
				}
			}
		}
	}
	return res;
}
 
template <class T>
DynMatrix<T>&
DynMatrix<T>::invert() {
	this->internalInvert();
	return *this;
}
 
template <class T>
void
DynMatrix<T>::mergeSortedRows(
 const DynMatrix<T>& a,
 const unsigned int iCol,
 const unsigned int lCol) {
	assert(this != &a);
	assert(lCol >= iCol);
	DynMatrix<T> b;
	b = *this;
	assert(a.nCols() == b.nCols());
	unsigned int m = a.nRows();
	unsigned int n = b.nRows();
	_nRows = m + n;
	_nCols = a.nCols();
	resizeVal(a.nRows() + b.nRows(), a.nCols());
	unsigned int i = 0, j = 0, k = 0;
	unsigned int vS = lCol - iCol + 1;
	while (i < m && j < n) {
		const T *aVal = a.valPtr(i,iCol);
		const T *bVal = b.valPtr(j,iCol);
		bool leq = this->isLEQ(aVal, bVal, vS);
		if (leq) {
			for (unsigned int col = 0; col < _nCols; col++) {
				val(k,col) = a(i,col);
			}
			i++;
		} else {
			for (unsigned int col = 0; col < _nCols; col++) {
				val(k,col) = b(j,col);
			}
			j++;
		}
        k++;
	}
	if (i < m) {
		for (unsigned int p = i; p < m; p++,k++) {
			for (unsigned int col = 0; col < _nCols; col++) {
				val(k,col) = a(p,col);
			}
		}
	} else {
		for (unsigned int p = j; p < n; p++,k++) {
			for (unsigned int col = 0; col < _nCols; col++) {
				val(k,col) = b(p,col);
			}
		}
	}
}

template <class T>
DynMatrix<T>&
DynMatrix<T>::transpose() {
	DynMatrix<T> temp = *this;
	// Assigns new values to _nRows and _nCols.
	_nRows = temp._nCols;
	_nCols = temp._nRows;
	// Assigns values to transposed matrix.
	for (unsigned int i = 0; i < _nRows; i++) {
		for (unsigned int j = 0; j < _nCols; j++) {
			_val[i][j] = temp._val[j][i];
		}
	}
	return *this;
}
// =============== StaMatrix methods ==========================================
// =-=-=-=-=-=-=-= Constructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>::StaMatrix() {
	for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
		_val[i] = T(0);
	}
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>::StaMatrix(
 const StaMatrix<T,NROWS,NCOLS>& param) {
	unsigned int nRnC = NROWS * NCOLS;
	for (unsigned int i = 0; i < nRnC; i++) {
		_val[i] = (T) param._val[i];
	}
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>::~StaMatrix() {
}
// =-=-=-=-=-=-=-= General methods =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
	if (this == &param)
		return *this;
	for (unsigned int i = 0; i < NROWS*NCOLS; i++)
		_val[i] = param._val[i];
	return *this;
}
 
template <class T, unsigned int NROWS, unsigned int NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::val(const unsigned int i) const {
	return _val[i];
}
 
template <class T, unsigned int NROWS, unsigned int NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::val(const unsigned int i) {
	return _val[i];
}
 
template <class T, unsigned int NROWS, unsigned int NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::val(const unsigned int row, const unsigned int col) const {
	return _val[row * NCOLS + col];
}
 
template <class T, unsigned int NROWS, unsigned int NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::val(const unsigned int row, const unsigned int col) {
	return _val[row * NCOLS + col];
}

template <class T, unsigned int NROWS, unsigned int NCOLS>
inline const T*
StaMatrix<T,NROWS,NCOLS>::val() const {
	return _val;
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
inline T
StaMatrix<T,NROWS,NCOLS>::operator()(
 const unsigned int row,
 const unsigned int col) const {
	return _val[row * NCOLS + col];
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
inline T&
StaMatrix<T,NROWS,NCOLS>::operator()(const unsigned int row, const unsigned int col) {
	return _val[row * NCOLS + col];
}
 
template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::assign(
 const StaMatrix<int,NROWS,NCOLS>& param) {
	for (unsigned int i = 0; i < NROWS; i++) {
		for (unsigned int j = 0; j < NCOLS; j++) {
			val(i,j) = param(i,j);
		}
	}
	return *this;
}
 
template <class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(const DynMatrix<double>& param) {
	assert(NROWS == param.nRows() && NCOLS == param.nCols());
	for (unsigned int i = 0; i < NROWS; i++)
		for (unsigned int j = 0; j < NCOLS; j++)
			val(i,j) = param(i,j);
	return *this;
}
 
template <class T, unsigned int NROWS, unsigned int NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::printInfo() const {
	printInfo(NROWS, NCOLS);
}
 
template <class T, unsigned int NROWS, unsigned int NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::printInfo(unsigned int rows, unsigned int cols) const {
	unsigned int i, j;
	if (rows > NROWS || cols > NCOLS) {
		cerr << "ERROR: Unable print more rows or cols than available" << endl;
		cerr << "Terminating."                                         << endl;
		exit(MATRICES_ERROR);
	}
	cout << "Matrix type: Double";
	cout << "Dimensions: " << NROWS << "x" << NCOLS << endl;
	cout << "Stored values: " << endl;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++)
			cout << val(i,j) << " ";
		cout << endl;
	}
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NCOLS,NROWS>&
StaMatrix<T,NROWS,NCOLS>::invert() {
	this->internalInvert();
	return *this;
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
inline unsigned int
StaMatrix<T,NROWS,NCOLS>::nCols() const {
	return NCOLS;
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
inline unsigned int
StaMatrix<T,NROWS,NCOLS>::nRows() const {
	return NROWS;
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NCOLS,NROWS>
StaMatrix<T,NROWS,NCOLS>::transpose() {
	StaMatrix<T,NCOLS,NROWS> res;
	for (unsigned int i = 0; i < NROWS; i++) {
		for (unsigned int j = 0; j < NCOLS; j++) {
			res(j,i) = val(i,j);
		}
	}
	return res;
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator+=(const double param) {
	for (unsigned int i = 0; i < NROWS*NCOLS; i++)
		_val[i] += param;
	return *this;
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator+=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
	for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
		_val[i] += param._val[i];
	}
	return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator-=(
 const StaMatrix<T,NROWS,NCOLS>& param) {
	for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
		_val[i] -= param._val[i];
	}
	return *this;
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator*=(const T param) {
	for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
		_val[i] *= param;
	}
	return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator/=(const T param) {
	for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
		_val[i] /= param;
	}
	return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>
StaMatrix<T,NROWS,NCOLS>::operator*(T param) const {
	StaMatrix<T,NROWS,NCOLS> res;
	for (unsigned int i = 0; i < NROWS*NCOLS; i++)
		res._val[i] = _val[i] * param;
	return res;
}
 
template<class T, unsigned int NROWS, unsigned NCOLS>
bool
StaMatrix<T,NROWS,NCOLS>::operator==(
 const StaMatrix<T,NROWS,NCOLS>& param) const {
	for (unsigned int i = 0; i < NROWS*NCOLS; i++) {
		if (_val[i] != param._val[i]) {
			return false;
		}
	}
	return true;
}
 
template<class T, unsigned int NROWS, unsigned NCOLS>
bool
StaMatrix<T,NROWS,NCOLS>::operator<(
 const StaMatrix<T,NROWS,NCOLS>& param) const {
	for (uint i = 0; i < (NROWS*NCOLS); i++) {
		if (val(i) < param.val(i)) {
			return true;
		}
		if (val(i) > param.val(i)) {
			return false;
		}
	}
	return false;
}
 
template<class T, unsigned int NROWS, unsigned int NCOLS>
StaMatrix<T,NROWS,NCOLS>&
StaMatrix<T,NROWS,NCOLS>::operator=(const DynMatrix<int>& rhs) {
	assert(NROWS == rhs.nRows() && NCOLS == rhs.nCols());
	for (unsigned int i = 0; i < NROWS; i++)
		for (unsigned int j = 0; j < NCOLS; j++)
			val(i,j) = (T) rhs(i,j);
	return *this;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
void
StaMatrix<T,NROWS,NCOLS>::copy(vector<vector<T> > values) {
	assert(NROWS == values.size());
	for (unsigned int i = 0; i < NROWS; i++) {
		assert(values[i].size() == NCOLS);
		for (unsigned int j = 0; j < NCOLS; j++) {
			val(i,j) = values[i][j];
		}
	}
}

// =============== Not member functions =======================================
template<class T, class S, unsigned int NROWS, unsigned int NCOLS, unsigned int NCOLSB>
StaMatrix<T,NROWS,NCOLSB>
operator*(const StaMatrix<T,NROWS,NCOLS>& lhs,
          const StaMatrix<S,NCOLS,NCOLSB>& rhs) {
	unsigned int i, j, k;
	StaMatrix<T,NROWS,NCOLSB> res;
	for (i = 0; i < NROWS; i++) {
		for (k = 0; k < NCOLS; k++) {
			for (j = 0; j < NCOLSB; j++) {
				res(i,j) += lhs(i,k) * rhs(k,j);
			}
		}
	}
	return res;
}

template<class T, unsigned int NROWS, unsigned int NCOLS>
DynMatrix<T>
operator*(const StaMatrix<T,NROWS,NCOLS>& lhs, const DynMatrix<T>& rhs) {
	assert(NCOLS == rhs.nRows());
	unsigned int i, j, k;
	DynMatrix<T> res(NROWS, rhs.nCols());
	for (i = 0; i < NROWS; i++) {
		for (k = 0; k < NCOLS; k++) {
			for (j = 0; j < rhs.nCols(); j++) {
				res(i,j) += lhs(i,k) * rhs(k,j);
			}
		}
	}
	return res;
}
