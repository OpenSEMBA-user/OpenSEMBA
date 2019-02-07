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

#include "Polynomial.h"

namespace SEMBA {
namespace Math {
namespace Function {

template<class T>
Polynomial<T>::Polynomial() {
    nv_ = 0;
    nm_ = 0;
}
template<class T>
Polynomial<T>::Polynomial(const std::size_t nvar) {
    nv_ = nvar;
    nm_ = 0;
}

template<class T>
inline Polynomial<T>::~Polynomial() {
}

template<class T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T> &param) {
    if (this == &param)
        return *this;
    nv_ = param.nv_;
    nm_ = param.nm_;
    mv_ = param.mv_;
    mp_ = param.mp_;
    return *this;
}

template<class T>
inline std::size_t Polynomial<T>::numberOfVariables() const {
    return nv_;
}

template<class T>
inline std::size_t Polynomial<T>::numberOfMonomials() const {
    return nm_;
}

template<class T>
inline T Polynomial<T>::monomialValue(const std::size_t i) const {
    return mv_[i];
}

template<class T>
inline Int Polynomial<T>::monomialPower(
        const std::size_t monomial,
        const std::size_t variable) const {
    return mp_[monomial][variable];
}

template<class T>
T Polynomial<T>::eval(const Vector::Cartesian<T,1>& pos) const {
    assert(1 == nv_);
    T prod;
    std::size_t m, v;
    T res = 0.0;
    for (m = 0; m < nm_; m++) {
        prod = 1.0;
        for (v = 0; v < nv_; v++)
            prod *= pow(pos(v), mp_[m][v]);
        res += mv_[m] * prod;
    }
    return res;
}

template<class T>
T Polynomial<T>::eval(const Vector::Cartesian<T,2>& pos) const {
    assert(2 == nv_);
    T prod;
    std::size_t m, v;
    T res = 0.0;
    for (m = 0; m < nm_; m++) {
        prod = 1.0;
        for (v = 0; v < nv_; v++)
            prod *= pow(pos(v), mp_[m][v]);
        res += mv_[m] * prod;
    }
    return res;
}

template<class T>
T Polynomial<T>::eval(const Vector::Cartesian<T,3>& pos) const {
    assert(3 == nv_);
    T prod;
    std::size_t m, v;
    T res = 0.0;
    for (m = 0; m < nm_; m++) {
        prod = 1.0;
        for (v = 0; v < nv_; v++)
            prod *= pow(pos(v), mp_[m][v]);
        res += mv_[m] * prod;
    }
    return res;
}

template<class T>
T Polynomial<T>::eval(const Vector::Cartesian<T,4>& pos) const {
    assert(4 == nv_);
    T prod;
    std::size_t m, v;
    T res = 0.0;
    for (m = 0; m < nm_; m++) {
        prod = 1.0;
        for (v = 0; v < nv_; v++)
            prod *= pow(pos(v), mp_[m][v]);
        res += mv_[m] * prod;
    }
    return res;
}

template<class T>
void Polynomial<T>::derive(std::size_t coord) {
    // Performs derivative with respect to coordinate coord.
    for (std::size_t m = 0; m < nm_; m++)
        if (mp_[m][coord] == 0)
            mv_[m] = 0.0;
        else {
            mv_[m] *= mp_[m][coord];
            mp_[m][coord]--;
        }
    // Removes monomials with value zero.
    removeZeros();
}

template<class T>
void Polynomial<T>::removeZeros() {
    for (std::size_t i = 0; i < nm_; i++)
        if (mv_[i] == 0) {
            nm_--;
            mv_.erase(mv_.begin() + i);
            mp_.erase(mp_.begin() + i);
        }
}

template<class T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T> &param) const {
    // PURPOSE:
    // Performs polynomial product.
    // Algebraically, convolution is the same operation as multiplying
    assert(nv_ == 1 && param.nv_ == 1);
    Matrix::Dynamic<T> a, b, c;
    Polynomial<T> res(1);
    // Converts polynomials to vectors.
    a = polynomialToMatrix();
    b = param.polynomialToMatrix();
    // Performs product as vector convolution.
    c = a.convolute(b);
    // Copies result in this polynomial.
    res = matrixToPolynomial(c);
    // Returns result.
    return res;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial<T> &param) {
    assert(nv_ == 1 && param.nv_ == 1);
    Matrix::Dynamic<T> a, b, c;
    // Converts polynomials to vectors.
    a = polynomialToMatrix();
    b = param.polynomialToMatrix();
    // Performs product as vector convolution.
    c = a.convolute(b);
    // Copies ;result in this polynomial.
    *this = matrixToPolynomial(c);
    // Returns result.
    return *this;
}

template<class T>
Polynomial<T> Polynomial<T>::operator^(const Polynomial<T> &param) {
    // PURPOSE: Performs external product between polynomials. This means that
    // variables in both polynomials will be treated as if they are different.
    std::size_t i, j;
    Polynomial<T> res(nv_ + param.nv_);
    std::vector<Int> pow;
    for (i = 0; i < nm_; i++)
        for (j = 0; j < param.nm_; j++) {
            pow = mp_[i];
            pow.insert(pow.end(), param.mp_[j].begin(), param.mp_[j].end());
            res.addMonomial(mv_[i]*param.mv_[j], pow);
        }
    return res;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator^=(const Polynomial<T> &param) {
    // PURPOSE: Performs external product between polynomials. This means that
    // variables in both polynomials will be treated as if they are different.
    std::size_t i, j;
    Polynomial<T> res(nv_ + param.nv_);
    std::vector<Int> pow;
    for (i = 0; i < nm_; i++)
        for (j = 0; j < param.nm_; j++) {
            pow = mp_[i];
            pow.insert(pow.end(), param.mp_[j].begin(), param.mp_[j].end());
            res.addMonomial(mv_[i]*param.mv_[j], pow);
        }
    *this = res;
    return *this;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator/=(const T param) {
    assert(param != 0);
    for (std::size_t i = 0; i < nm_; i++)
        mv_[i] /= param;
    return *this;
}

template<class T>
Polynomial<T> Polynomial<T>::vectorToPolynomial(T *v,
                                                std::size_t sv,
                                                std::size_t nvar) {
    assert(nvar == 1);
    Polynomial<T> res(1);
    for (Int i = 0; i < sv; i++)
        if (v[i] != 0)
            res.addMonomial(v[i], i);
    return res;
}

template<class T>
Polynomial<T> Polynomial<T>::matrixToPolynomial (
        const Matrix::Dynamic<T> &param) const {
    assert(nv_ <= 2);
    Polynomial<T> res;
    if (param.nCols() == 1) {
        res.nv_ = 1;
        for (std::size_t i = 0; i < param.nRows(); i++)
            if (param(i,0) != 0.0)
                res.addMonomial(param(i,0), i);
    } else {
        std::vector<Int> pow(2,0);
        res.nv_ = 2;
        for (std::size_t i = 0; i < param.nRows(); i++)
            for (std::size_t j = 0; j < param.nCols(); j++)
                if (param(i,j) != 0.0) {
                    pow[0] = i;
                    pow[1] = j;
                    res.addMonomial(param(i,j), pow);
                }
    }
    return res;
}

template<class T>
Matrix::Dynamic<T> Polynomial<T>::polynomialToMatrix() const {
    assert(nv_ == 1);
    Matrix::Dynamic<T>  res(maxPower() + 1, 1);
    // Copies monomials to vector positions.
    for (std::size_t i = 0; i < nm_; i++)
        res(mp_[i][0], 0) = mv_[i];
    return res;
}

template<class T>
void Polynomial<T>::addMonomial(T val, std::vector<Int> pow) {
    assert(nv_ == pow.size());
    nm_++;
    mv_.push_back(val);
    mp_.push_back(pow);
}

template<class T>
void Polynomial<T>::addMonomial(T val, Int pow) {
    assert(nv_ == 1);
    std::vector<Int> vPow(1, pow);
    nm_++;
    mv_.push_back(val);
    mp_.push_back(vPow);
}

template<class T>
Int Polynomial<T>::maxPower() const {
    // Returns maximum power present in this polynomial.
    std::size_t i, j;
    Int res = 0;
    for (i = 0; i < nv_; i++)
        for (j = 0; j < nm_; j++)
            if (mp_[j][i] > res)
                res = mp_[j][i];
    return res;
}

template<class T>
inline T Polynomial<T>::operator ()(const T& arg) const {
    return eval(Vector::Cartesian<T,1>(arg));
}

template<class T>
inline bool Polynomial<T>::operator ==(const Base& rhs) const {
    if (dynamic_cast<const Polynomial<T>*>(&rhs) == nullptr) {
        return false;
    }
    const Polynomial<T>* rhsPol = dynamic_cast<const Polynomial<T>*>(&rhs);
    bool areEqual = true;
    areEqual &= mv_ == rhsPol->mv_;
    areEqual &= mp_ == rhsPol->mp_;
    return areEqual;
}

template<class T>
void Polynomial<T>::printInfo() const {
    std::cout << " -- Polynomial<T> -- " << std::endl;
    std::cout << "Number of variables: " << nv_ << std::endl;
    std::cout << "Number of monomials: " << nm_ << std::endl;
    std::cout << "Value             Powers" << std::endl;
    std::size_t i, j;
    for (i = 0; i < nm_; i++) {
        std::cout << mv_[i] << "               ";
        for (j = 0; j < nv_; j++)
            std::cout << mp_[i][j] << " ";
        std::cout << std::endl;
    }
}

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */

