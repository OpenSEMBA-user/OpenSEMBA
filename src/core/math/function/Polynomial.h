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

#ifndef SEMBA_MATH_FUNCTION_POLYNOMIAL_H_
#define SEMBA_MATH_FUNCTION_POLYNOMIAL_H_

#include <vector>

#include "math/matrix/Dynamic.h"
#include "math/vector/Cartesian.h"

#include "Function.h"

namespace SEMBA {
namespace Math {
namespace Function {

template<class T>
class Polynomial : public Function<T,T>{
public:
    Polynomial();
    Polynomial(const UInt nvar);
    virtual ~Polynomial();

    SEMBA_MATH_FUNCTION_DEFINE_CLONE(Polynomial);

    UInt numberOfVariables() const;
    UInt numberOfMonomials() const;

    T monomialValue(const UInt i) const;
    UInt monomialPower(const UInt monomial, const UInt variable) const;

    Polynomial<T>& operator=(const Polynomial<T> &param);

    bool operator==(const Base& rhs) const;

    // Product between polynomials assuming variables are the same.
    Polynomial<T> operator*(const Polynomial<T> &param ) const;
    Polynomial<T>& operator*=(const Polynomial<T> &param);

    // Product between polynomials assuming variables are different.
    Polynomial<T> operator^(const Polynomial<T> &param);
    Polynomial<T>& operator^=(const Polynomial<T> &param);

    Polynomial<T>& operator/=(const T param );
    void addMonomial(T val, std::vector<Int> pow);
    void addMonomial(T val, Int pow);
    Int maxPower() const;

    T operator()(const T&) const;
    T eval(const Vector::Cartesian<T,1>& pos) const;
    T eval(const Vector::Cartesian<T,2>& pos) const;
    T eval(const Vector::Cartesian<T,3>& pos) const;
    T eval(const Vector::Cartesian<T,4>& pos) const;

    void derive(Int coord);

    Polynomial<T> vectorToPolynomial(T *v,  Int sv, Int nvar);
    Polynomial<T> matrixToPolynomial(const Matrix::Dynamic<T> &param) const;
    Matrix::Dynamic<T> polynomialToMatrix() const;
    void removeZeros();

    void printInfo() const;
private:
    UInt nv_; // Number of variables.
    UInt nm_; // Number of monomials.
    std::vector<T> mv_; // Value of constant monomials coefficients.
    std::vector<std::vector<Int>> mp_; // nm x nv array, containing powers of monomials.
};

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */

#include "Polynomial.hpp"

#endif /* SEMBA_MATH_FUNCTION_POLYNOMIAL_H_ */
