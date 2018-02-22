// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#include <cassert>
#include <stdexcept>
#include <numeric>

#include "Matrix.h"

namespace SEMBA {
namespace Math {
namespace Matrix {

template <class T>
Matrix<T>::Matrix() {

}

template <class T>
Matrix<T>::~Matrix() {

}

template <class T>
std::size_t Matrix<T>::size() const {
    return (this->nRows()*this->nCols());
}

template <class T>
T Matrix<T>::getDeterminant3x3() const {
    assert(nRows() == 3 && nCols() == 3);
    return + val(0,0) * (val(1,1)*val(2,2) - val(2,1)*val(1,2))
    - val(0,1) * (val(1,0)*val(2,2) - val(1,2)*val(2,0))
    + val(0,2) * (val(1,0)*val(2,1) - val(1,1)*val(2,0));
}

template <class T>
T Matrix<T>::getDeterminant4x4() const {
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
void Matrix<T>::internal_() {
    assert(Matrix<T>::isSquare());
    std::size_t *pivot = new std::size_t[nRows()];
    factorizeLU_(pivot);
    invertFactorized_(pivot);
    delete [] pivot;
}

template<class T>
void Matrix<T>::factorizeLU_(std::size_t pivot[]) {
    // Performs a LINPACK-style PLU factorization of a general matrix.
    // Output, std::size_t PIVOT[N], a vector of pivot indices.
    assert(nRows() == nCols());
    std::size_t n = nRows();
    std::size_t i, j, k, l;
    T t;
    for (k = 0; k < n-1; k++) {
        //  Find L, the index of the pivot row.
        l = k;
        for (i = k; i < n; i++) {
            if (std::abs(val(l,k)) < std::abs(val(i,k))) {
                l = i;
            }
        }
        pivot[k] = l;
        //  If the pivot index is zero, the algorithm has failed.
        if (val(l,k) == (T) 0.0) {
            printInfo();
            std::stringstream ss;
            ss << "Zero pivot on step " << k;
            throw std::out_of_range(ss.str());
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
            if (l != k) {
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
        std::cout << "ERROR@Matrix<T>::factorizeToArray" << std::endl;
        std::cout << "  Zero pivot on step " << n-1 << std::endl;
        printInfo();
        assert(false);
        exit (EXIT_FAILURE);
    }
}

template <class T>
void Matrix<T>::invertFactorized_(const std::size_t pivot[]) {
    // Computes inverse of a general matrix factored by factorize.
    // Input, std::size_t PIVOT(N), the pivot vector from R8GE_FA.
    assert(nRows() == nCols());
    std::ptrdiff_t n = nRows();
    std::ptrdiff_t nn = n * n;
    std::ptrdiff_t i, j, k;
    T temp;
    T *b = new T[nn];
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
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            val(i,j) = (T) b[i+j*n];
        }
    }
    // Frees memory.
    delete [] b;
}

template <class T>
T Matrix<T>::maxVal() const {
    T res = val(0,0);
    for (std::size_t i = 0; i < nRows(); i++) {
        for (std::size_t j = 0; j < nCols(); j++) {
            if (val(i,j) > res)
                res = val(i,j);
        }
    }
    return res;
}

template <class T>
T Matrix<T>::maxValInCol(std::size_t col) const {
    T res = val(0,col);
    for (std::size_t i = 0; i < nRows(); i++)
        if (val(i,col) > res)
            res = val(i,col);
    return res;
}

template <class T>
void Matrix<T>::sortRows() {
    // Orders array a with nr rows and nc columns. Ordering is performed
    // using column indicated in orCol as reference. In ascending order.
    sortRows(0, nCols());
}

template <class T>
void Matrix<T>::sortRows(const std::size_t iCol, const std::size_t lCol) {
    // Orders array a with nr rows and nc columns. Ordering is performed
    // using column indicated in orCol as reference. In ascending order.
    if (nRows() <= 1) {
        return;
    }
    QSRows_(0, nRows()-1, iCol, lCol);
}

template <class T>
bool Matrix<T>::isEQ_(const T* x1,
                      const T* x2,
                      const std::size_t vS) const {
    for (std::size_t i = 0; i < vS; i++) {
        if (x1[i] != x2[i]) {
            return false;
        }
    }
    return true;
}

template <class T>
bool Matrix<T>::isGEQ_(const T* x1,
                       const T* x2,
                       const std::size_t vS) const {
    for (std::size_t i = 0; i < vS; i++) {
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
bool Matrix<T>::isLEQ_(const T* x1,
                       const T* x2,
                       const std::size_t vS) const {
    for (std::size_t i = 0; i < vS; i++) {
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
std::size_t Matrix<T>::partitionRows_(std::size_t p, std::size_t r,
                                      const std::size_t iCol,
                                      const std::size_t lCol) {
    std::size_t j = p - 1;
    for (std::size_t i = p; i < r; i++) {
        bool geq = isGEQ_(&val(r,iCol), &val(i,iCol), lCol - iCol + 1);
        if (geq) {
            j++;
            for (std::size_t k = 0; k < nCols(); k++) {
                T temp = val(j,k);
                val(j,k) = val(i,k);
                val(i,k) = temp;
            }
        }
    }
    for (std::size_t k = 0; k < nCols(); k++) {
        T x = val(r, k);
        val(r, k) = val(j+1, k);
        val(j+1, k) = x;
    }
    return (j + 1);
}

template <class T>
void Matrix<T>::QSRows_(std::size_t p, std::size_t r,
                        const std::size_t iCol, const std::size_t lCol) {
    if (p < r) {
        std::size_t q = partitionRows_(p, r, iCol, lCol);
        QSRows_(p, q - 1, iCol, lCol);
        QSRows_(q + 1, r, iCol, lCol);
    }
}

template <class T>
std::size_t Matrix<T>::binarySearch_(const T* key,
                                     const std::size_t col,
                                     const std::size_t vecSize,
                                     std::size_t imin,
                                     std::size_t imax) const {
    // NOTE: Just for ascending order!!!
    if (imax == imin) {
        if (nRows() == imin) {
            return imin;
        }
        // Checks that this value corresponds to the value searched.
        T *value;
        value = new T [vecSize];

        for (std::size_t i = 0; i < vecSize; i++) {
            value[i] = val(imin, col+i);
        }
        bool iseq;
        iseq = isEQ_(value, key, vecSize);
        delete[] value;
        if (iseq) {
            return imin;
        } else {
            return nRows();
        }
    } else {
        std::size_t imid = (std::size_t) (imin + imax) / 2;
        T *value;
        value = new T [vecSize];
        for (std::size_t i = 0; i < vecSize; i++) {
            value[i] = val(imid, col+i);
        }
        bool geq = isGEQ_(value, key, vecSize);
        delete[] value;
        if (geq) {
            return binarySearch_(key, col, vecSize, imin, imid);
        } else {
            return binarySearch_(key, col, vecSize, imid+1, imax);
        }
    }
}

template <class T>
std::size_t Matrix<T>::findFirstOcurrenceInColumns(
        const T* key,
        const std::size_t col,
        const std::size_t vecSize) const {
    // Performs binary search -------------------------------------------------
    std::size_t row = binarySearch_(key, col, vecSize, 0, nRows());
    if (row == nRows()) {
        return row; // Returns this is value was not found.
    }
    // Goes back to find the first occurrence ---------------------------------
    for (std::size_t i = row; i > 0; i--) {
        bool isEqual = true;
        for (std::size_t j = 0; j < vecSize; j++) {
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
void Matrix<T>::cpLowerTri2UpperTri() {
    std::size_t i, j;
    for (i = 0; i < nRows(); i++)
        for (j = i+1; j < nCols(); j++)
            val(i,j) = val(j,i);
}

template <class T>
void Matrix<T>::zeros() {
    for (std::size_t i = 0; i < nRows(); i++)
        for (std::size_t j = 0; j < nCols(); j++)
            val(i,j) = (T) 0;
}

template <class T>
void Matrix<T>::convertToArray(const Int mode, Real *res) const {
    if (mode == MATRICES_COL_MAJOR) {
        for (std::size_t j = 0; j < nCols(); j++)
            for (std::size_t i = 0; i < nRows(); i++)
                res[i + j * nRows()] = val(i,j);
        return;
    } else if (mode == MATRICES_ROW_MAJOR) {
        for (std::size_t i = 0; i < nRows(); i++)
            for (std::size_t j = 0; j < nCols(); j++)
                res[i * nCols() + j] = val(i,j);
        return;
    }
}

template <class T>
bool Matrix<T>::isSquare() const {
    return (nRows() == nCols());
}

template <class T>
bool Matrix<T>::isSymmetric() const {
    for (std::size_t i = 0; i < nRows(); i++) {
        for (std::size_t j = 0; j < nRows(); j++) {
            if (val(i,j) != val(j,i)) {
                return false;
            }
        }
    }
    return true;
}

template <class T>
void Matrix<T>::eye() {
    assert(isSquare());
    // Sets 1 in main diagonal or 0 otherwise.
    zeros();
    for (std::size_t i = 0; i < nRows(); i++)
        val(i,i) = T(1);
}

template <class T>
std::vector<T> Matrix<T>::cpRowToVector(const std::size_t row) const {
    std::vector<T> res(nCols());
    for (std::size_t i = 0; i < nCols(); i++) {
        res[i] = val(row,i);
    }
    return res;
}

template <class T>
void Matrix<T>::cpToCol(const size_t col, const std::vector<T>& vec) {
    assert(nRows() == vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        val(i, col) = vec[i];
    }
}

template <class T>
void Matrix<T>::cpToRow(const size_t row, const std::vector<T>& vec) {
    assert(nCols() == vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        val(row, i) = vec[i];
    }
}

template <class T>
std::vector<Vector::Cartesian<T,3> >
        Matrix<T>::convertToCartesianVector() const {
    assert(nCols() == 3);
    std::vector<Vector::Cartesian<T,3> > res(nRows());
    for (std::size_t r = 0; r < nRows(); r++) {
        res[r] = Vector::Cartesian<T,3>(val(r,0), val(r,1), val(r,2));
    }
    return res;
}

/**
 * Gram-Schmidt process to compute QR factorization
 * Algorithm and notation taken from http://sl.ugr.es/GramSchmidtProcess
 */
template<class T>
bool Matrix<T>::factorizeQR_(Matrix<T>& Q, Matrix<T>& R) {
    assert(nRows() == nCols());

    int nCols = this->nCols();

    std::vector< std::vector<T> > a;
    std::vector< std::vector<T> > u;
    std::vector< std::vector<T> > e;
    std::vector<T> aux_dot;

    T dot;

    for (int i = 0; i < nCols; i++) {
        // u_i = a_i
        a.push_back(this->cpRowToVector(i));
        u.push_back(a[i]);

        for (int j = 0; j <= i-1; j++) {
            // <a_i, e_j>
            dot = std::inner_product(a[i].begin(), a[i].end(), e[j].begin(), 0.0);

            aux_dot = e[j];

            // u_i -= <a_i, e_j> * e_j
            for (size_t k = 0; k < nCols; k++) {
                aux_dot[k] *= dot;
                u[i][k] -= aux_dot[k];
            }
        }

        // Compute e_i
        T norm_u = sqrt(std::inner_product(u[i].begin(), u[i].end(), u[i].begin(), 0.0));

        std::vector<T> ei;
        for (size_t j = 0; j < nCols; j++) {
            ei.push_back(u[i][j] / norm_u);
        }

        e.push_back(ei);
    }

    // Compute final Q and R matrices
    for (int i = 0; i < nCols; i++) {
        for (int j = 0; j < i; j++) {
            Q(j,i) = e[i][j];
            R(i,j) = 0;
        }

        for (int j = i; j < nCols; j++) {
            Q(j,i) = e[i][j];

            // <a_j, e_i>
            dot = std::inner_product(a[j].begin(), a[j].end(), e[i].begin(), 0.0);
            R(i,j) = dot;
        }
    }

    return true;
}

/**
 * Sums the absolute values of all the elements below the diagonal.
 * Currently (mis)used in computeEigenvalues_ function.
 * @returns An element of class T with the sum of the absolute (or norm
 *             if the elements are complex numbers) values of all the
 *             elements below the diagonal.
 */
template <class T>
T Matrix<T>::belowDiagonalAbsSum_(){
    T sum = abs(val(1,0));

    for (size_t i = 2; i < nRows(); i++) {
        for (size_t j = 0; j < i; j++) {
            sum += abs(this->val(i,j));
        }
    }

    return sum;
}

} /* namespace MAtrix */
} /* namespace Math */
} /* namespace SEMBA */
