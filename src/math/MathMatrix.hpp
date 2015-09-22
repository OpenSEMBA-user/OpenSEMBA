
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
	assert(MathMatrix<T>::isSquare());
	Int *pivot = new Int[nRows()];
	factorizeLU(pivot);
	invertFactorized(pivot);
	delete [] pivot;
}
 
template<class T>
void
MathMatrix<T>::factorizeLU(Int pivot[]) {
	// Performs a LINPACK-style PLU factorization of a general matrix.
	// Output, UInt PIVOT[N], a vector of pivot indices.
	assert(nRows() == nCols());
	UInt n = nRows();
	UInt i, j, k, l;
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
			exit (EXIT_FAILURE);
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
		exit (EXIT_FAILURE);
	}
}

template <class T>
void
MathMatrix<T>::invertFactorized(const Int pivot[]) {
	// Computes inverse of a general matrix factored by factorize.
	// Input, Int PIVOT(N), the pivot vector from R8GE_FA.
	assert(nRows() == nCols());
	Int n = nRows();
	Int nn = n * n;
	Int i, j, k;
	T temp;
	Real *b = new Real[nn];
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
	for (Int i = 0; i < n; i++) {
		for (Int j = 0; j < n; j++) {
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
	for (UInt i = 0; i < nRows(); i++) {
		for (UInt j = 0; j < nCols(); j++) {
			if (val(i,j) > res)
				res = val(i,j);
		}
	}
	return res;
}
 
template <class T>
T
MathMatrix<T>::maxValInCol(UInt col) const {
	T res = val(0,col);
	for (UInt i = 0; i < nRows(); i++)
		if (val(i,col) > res)
			res = val(i,col);
	return res;
}

template <class T>
void
MathMatrix<T>::randomize(T min, T max) {
	Real range = (Real) max - (Real) min;
	for (UInt i = 0; i < nRows(); i++) {
		for (UInt j = 0; j < nCols(); j++) {
			Real aux = range * rand() / (RAND_MAX + (Real) 1) + min;
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
MathMatrix<T>::sortRows(const UInt iCol, const UInt lCol) {
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
 const UInt vS) const {
	for (UInt i = 0; i < vS; i++) {
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
 const UInt vS) const {
	for (UInt i = 0; i < vS; i++) {
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
 const UInt vS) const {
	for (UInt i = 0; i < vS; i++) {
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
Int
MathMatrix<T>::partitionRows(
 Int p, Int r,
 const UInt iCol, const UInt lCol) {
	Int j = p - 1;
	for (Int i = p; i < r; i++) {
		bool geq = isGEQ(&val(r,iCol), &val(i,iCol), lCol - iCol + 1);
		if (geq) {
			j++;
			for (UInt k = 0; k < nCols(); k++) {
				T temp = val(j,k);
				val(j,k) = val(i,k);
				val(i,k) = temp;
			}
		}
	}
	for (UInt k = 0; k < nCols(); k++) {
		T x = val(r, k);
		val(r, k) = val(j+1, k);
		val(j+1, k) = x;
	}
	return (j + 1);
}

template <class T>
void
MathMatrix<T>::QSRows(
 Int p, Int r, const UInt iCol, const UInt lCol) {
	if (p < r) {
		Int q = partitionRows(p, r, iCol, lCol);
		QSRows(p, q - 1, iCol, lCol);
		QSRows(q + 1, r, iCol, lCol);
	}
}

template <class T>
UInt
MathMatrix<T>::binarySearch(
 const T* key,
 const UInt col,
 const UInt vecSize,
 UInt imin,
 UInt imax) const {
	// NOTE: Just for ascending order!!!
	if (imax == imin) {
		if (nRows() == imin) {
			return imin;
		}
		// Checks that this value corresponds to the value searched.
		T *value;
		value = new T [vecSize];

		for (UInt i = 0; i < vecSize; i++) {
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
		UInt imid = (UInt) (imin + imax) / 2;
		T *value;
		value = new T [vecSize];
		for (UInt i = 0; i < vecSize; i++) {
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
UInt
MathMatrix<T>::findFirstOcurrenceInColumns(
 const T* key,
 const UInt col,
 const UInt vecSize) const {
	// Performs binary search -------------------------------------------------
	UInt row = binarySearch(key, col, vecSize, 0, nRows());
	if (row == nRows()) {
		return row; // Returns this is value was not found.
	}
	// Goes back to find the first occurrence ---------------------------------
	for (UInt i = row; i > 0; i--) {
		bool isEqual = true;
		for (UInt j = 0; j < vecSize; j++) {
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
	UInt i, j;
	for (i = 0; i < nRows(); i++)
		for (j = i+1; j < nCols(); j++)
			val(i,j) = val(j,i);
}
 
template <class T>
void
MathMatrix<T>::zeros() {
	for (UInt i = 0; i < nRows(); i++)
		for (UInt j = 0; j < nCols(); j++)
 			val(i,j) = (T) 0;
}
 
template <class T>
void
MathMatrix<T>::convertToArray(const Int mode, Real *res) const {
	if (mode == MATRICES_COL_MAJOR) {
		for (UInt j = 0; j < nCols(); j++)
			for (UInt i = 0; i < nRows(); i++)
				res[i + j * nRows()] = val(i,j);
		return;
	} else if (mode == MATRICES_ROW_MAJOR) {
		for (UInt i = 0; i < nRows(); i++)
			for (UInt j = 0; j < nCols(); j++)
				res[i * nCols() + j] = val(i,j);
		return;
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
	for (UInt i = 0; i < nRows(); i++) {
		for (UInt j = 0; j < nRows(); j++) {
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
	for (UInt i = 0; i < nRows(); i++)
		val(i,i) = T(1);
}
 
template <class T>
vector<T>
MathMatrix<T>::cpRowToVector(const UInt row) const {
	vector<T> res(nCols());
	for (UInt i = 0; i < nCols(); i++) {
		res[i] = val(row,i);
	}
	return res;
}

template <class T>
vector<CartesianVector<T,3> >
MathMatrix<T>::convertToCartesianVector() const {
	assert(nCols() == 3);
	vector<CartesianVector<T,3> > res(nRows());
	for (UInt r = 0; r < nRows(); r++) {
		res[r] = CartesianVector<T,3>(val(r,0), val(r,1), val(r,2));
	}
	return res;
}
