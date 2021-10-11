

#pragma once

#include "Matrix.h"

namespace SEMBA {
namespace Math {
namespace Matrix {

template<class T>
class Dynamic : public Matrix<T> {
public:
   Dynamic();
   Dynamic(const Dynamic<T>& param);
   Dynamic(std::size_t rows, std::size_t cols);
   Dynamic(std::size_t rows, std::size_t cols, T** values);
   virtual ~Dynamic();
   std::size_t nCols() const;
   std::size_t nRows() const;
   T operator()(const std::size_t row, const std::size_t col) const;
   T& operator()(const std::size_t row, const std::size_t col);
   T val(const std::size_t ind) const;
   T& val(const std::size_t ind);
   T val(const std::size_t row, const std::size_t col) const;
   T& val(const std::size_t row, const std::size_t col);
   const T* valPtr(const std::size_t row, const std::size_t col) const;
   // ----------- assignment, copy -------------------------------------------
   Dynamic<T>& operator=(const Dynamic<T> &param);
   Dynamic<T>& operator=(const std::vector<T> &param);
   Dynamic<T>& operator=(const T &param);
   void copy(std::vector<std::vector<T> > values);
   void copy(const Dynamic<Real> &param);
   Dynamic<T> copy(const std::size_t fRow,
                   const std::size_t lRow,
                   const std::size_t fCol,
                   const std::size_t lCol) const;
   // ----------- operations -------------------------------------------------
   Dynamic<T> operator+(Dynamic<T> &param) const;
   Dynamic<T>& operator+=(const T param);
   Dynamic<T>& operator+=(const Dynamic& param);
   Dynamic<T>& operator*=(const T param);
   Dynamic<T>& operator/=(const T param);
   Dynamic<T> operator*(Dynamic<T> &param) const;
   Dynamic<T> operator*(const T param) const;
   Dynamic<T> operator/(const T param) const;
   Dynamic<T> kron(Dynamic<T>& param) const;
   Dynamic<T> sub(std::pair<std::size_t,std::size_t>& rows,
                  std::pair<std::size_t,std::size_t>& cols) const;
   bool operator==(const Dynamic<T>& param) const;
   bool operator<(const Dynamic<T>& param) const;
   Dynamic<T> convolute(const Dynamic<T>& param) const;
   Dynamic<T> invert();
   Dynamic<T>& transpose();
   Dynamic<T> reshape(std::size_t rows, std::size_t cols);
   Dynamic<T> reshape(std::size_t rows, std::size_t cols, Int order);
   Dynamic<T> eliminateColumns(std::size_t first, std::size_t last) const;
   Dynamic<T>& sortAndRemoveRepeatedRows_omp();

   void sortAndRemoveRepeatedRows();
   void removeRepeatedSortedRows();
   void sortRows_omp(const std::size_t iCol, const std::size_t lCol);
   void mergeSortedRows(const Dynamic<T>& a,
                        const std::size_t iCol,
                        const std::size_t lCol);

private:
   T* _val;
   std::size_t _nRows, _nCols;

   void resizeVal(const std::size_t rows, const std::size_t cols);
};

} /* namespace Matrix */
} /* namespace Math */
} /* namespace SEMBA */

#include "Dynamic.hpp"

