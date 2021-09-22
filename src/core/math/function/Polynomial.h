

#pragma once

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
    Polynomial(const std::size_t nvar);
    virtual ~Polynomial();

    SEMBA_MATH_FUNCTION_DEFINE_CLONE(Polynomial);

    std::size_t numberOfVariables() const;
    std::size_t numberOfMonomials() const;

    T monomialValue(const std::size_t i) const;
    Int monomialPower(const std::size_t monomial,
                      const std::size_t variable) const;

    Polynomial<T>& operator=(const Polynomial<T> &param);

    bool operator==(const Base& rhs) const;

    // Product between polynomials assuming variables are the same.
    Polynomial<T>  operator* (const Polynomial<T> &param) const;
    Polynomial<T>& operator*=(const Polynomial<T> &param);

    // Product between polynomials assuming variables are different.
    Polynomial<T>  operator^ (const Polynomial<T> &param);
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

    void derive(std::size_t coord);

    Polynomial<T> vectorToPolynomial(T *v, std::size_t sv, std::size_t nvar);
    Polynomial<T> matrixToPolynomial(const Matrix::Dynamic<T> &param) const;
    Matrix::Dynamic<T> polynomialToMatrix() const;
    void removeZeros();

    void printInfo() const;
private:
    std::size_t nv_; // Number of variables.
    std::size_t nm_; // Number of monomials.
    std::vector<T> mv_; // Value of constant monomials coefficients.
    std::vector<std::vector<Int>> mp_; // nm x nv array, containing powers of monomials.
};

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */

#include "Polynomial.hpp"

