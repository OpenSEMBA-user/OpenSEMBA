
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
