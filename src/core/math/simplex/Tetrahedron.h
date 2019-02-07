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

#ifndef SEMBA_MATH_SIMPLEX_TETRAHEDRON_H_
#define SEMBA_MATH_SIMPLEX_TETRAHEDRON_H_

#include "Simplex.h"
#include "Triangle.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

template <size_t N>
class Tetrahedron : public Simplex {
    friend class Triangle<N>;
public:
    static const std::size_t faces = 4;
    static const std::size_t nsc = 4;
    static const std::size_t n = N;
	static const std::size_t dimension = 3;
	#define TET_NP ((N + 1)*(N + 2)*(N + 3) / 6)
	static const std::size_t np = TET_NP;
	#define TET_NFP ((N+1)*(N+2)/2)
	static const std::size_t nfp = TET_NFP;
	static constexpr Real sizeFactor = 1.0 / 6.0;

    typedef Vector::Cartesian<size_t,nsc> Index;

    Tetrahedron();

    std::size_t vertex(const std::size_t) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t sideNode(const std::size_t f, const std::size_t i) const;

    std::size_t nodeIndex(const std::size_t node,
                          const std::size_t coordinate) const;

    const Function::Polynomial<Real>& getLagr(const std::size_t i) const;
    const Function::Polynomial<Real>& getDLagr(const std::size_t i, 
                                               const std::size_t f) const;
    std::vector<Real> getWeights() const;

    CVecR4 coordinate(const std::size_t i) const;
    CVecR4 sideCoordinate(const std::size_t f, const std::size_t i) const;

    void printInfo() const;

private:
    const Triangle<N> tri;

    std::array<Index,np> indices;
    Vector::Cartesian<Real,nsc> nodePositions[np];
    std::array<Real,np> weights;

    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][faces];

    Matrix::Static<Int, faces, TET_NFP> sNId;
    Matrix::Static<Int, TET_NFP, TET_NP> RMatrix(const std::size_t s) const;
    Matrix::Static<Int, TET_NP,  TET_NP> PMatrix(const std::size_t s) const;

    static size_t numberOfNodes(size_t order);
};

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Tetrahedron.hpp"

#endif /* SEMBA_MATH_SIMPLEX_TETRAHEDRON_H_ */
