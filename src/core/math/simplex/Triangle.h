
#pragma once

#include "math/vector/Cartesian.h"
#include "math/matrix/Static.h"
#include "math/function/Polynomial.h"

#include "Simplex.h"

#include <algorithm>

namespace SEMBA {
namespace Math {
namespace Simplex {

template <size_t N>
class Triangle : public Simplex {
public:
    static const std::size_t faces = 3;
    static const std::size_t nsc = 3;
    static const std::size_t dimension = 2;
    static const std::size_t vertices = 3;
#define TRI_NFP (N+1)
    static constexpr std::size_t nfp = TRI_NFP;
#define TRI_NP ((N + 1)*(N + 2) / 2)
    static constexpr std::size_t np  = TRI_NP;
    static constexpr Real sizeFactor = 1.0 / 2.0;

    typedef Vector::Cartesian<size_t,nsc> Index;

    Triangle();
    std::size_t vertex(const std::size_t i) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t sideNode  (const std::size_t f, const std::size_t i) const;

    std::size_t nodeIndex(const std::size_t i, const std::size_t j) const;

    Math::CVecR3 coordinate(const std::size_t i) const;

    const Function::Polynomial<Real>& getLagr(const std::size_t i) const;
    const Function::Polynomial<Real>& getDLagr(const std::size_t i,
                                               const std::size_t f) const;
    std::vector<Real> getWeights() const;

    void printInfo() const;

    static Matrix::Dynamic<Int> PMatrix(const std::size_t n,
                                        const std::size_t s);

private:
    Index indices[np];
    Matrix::Static<Int,faces,nfp> sideNodes;

    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][faces];

    Vector::Cartesian<Real,nsc> nodePositions[np];
    std::array<Real,np>         weights;

    Matrix::Static<Int,TRI_NFP,TRI_NP> RMatrix(const std::size_t s) const;

    static size_t numberOfNodes(size_t order);
};

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Triangle.hpp"

