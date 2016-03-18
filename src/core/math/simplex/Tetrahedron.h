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
public:
    static const std::size_t n = N;
    static const std::size_t nfp = (N+1)*(N+2)/2;
    static const std::size_t np = (N+1)*(N+2)*(N+3) / 6;
    static const std::size_t nsc = 4;

    Tetrahedron();

    std::size_t order() const {return n;}
    std::size_t numberOfSimplexCoordinates() const {return nsc;}
    std::size_t numberOfNodes() const {return np;}
    std::size_t numberOfFaceNodes() const {return nfp;}

    std::size_t vertex(const std::size_t) const;
    std::size_t sideNode(const std::size_t f, const std::size_t i) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t nodeIndex(const std::size_t i, const std::size_t j) const;

    const Function::Polynomial<Real>& getLagr(const std::size_t i) const;
    const Function::Polynomial<Real>& getDLagr(const std::size_t i, 
                                               const std::size_t f) const;

    CVecR4 coordinate(const std::size_t i) const;
    CVecR4 sideCoordinate(const std::size_t f, const std::size_t i) const;

    Real integrateScalarsOnFace(const Real x[np],
                                const std::size_t f,
                                const Real area) const;
    Real integrateScalars(const Real x[np],
                          const Real volume) const;

    Real getCda(std::size_t i, std::size_t j, std::size_t k) const;
    Real getWeight(const size_t node) const;

    Matrix::Static<Real,np,np> getMassMatrix() const;

    void printInfo() const;

protected:
    Matrix::Static<Int,nfp,np> RMatrix(const std::size_t s) const;
    Matrix::Dynamic<Int> PMatrix_(const std::size_t n,
                                 const std::size_t s) const;

private:
    static const Real sizeFactor;
    static const std::size_t dimension = 3;
    const Triangle<N> tri;

    Vector::Cartesian<Int,nsc> nId[np];
    Matrix::Static<Int,nsc,nfp> sNId;
    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][nsc];

    Vector::Cartesian<Real,nsc> cPos[np];
    Real cw[np];
    Real ca[np][np];
    Real cda[np][nsc][np];
    Matrix::Static<Real,np,np> cwaa[np];
    Matrix::Static<Real,np,np> cwada[np][nsc];
    Matrix::Static<Real,np,nfp> LIFT[nsc];


    void buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                          const std::size_t order,
                          const std::size_t nNodes) const;
    void buildSideNodeIndices();

    static size_t numberOfNodes(size_t order);

    void buildCubaturePositionsAndWeights();
    void buildCubedLagrange_();

    void buildLIFTMatrix();
};

template <size_t N>
const size_t Tetrahedron<N>::n;
template <size_t N>
const size_t Tetrahedron<N>::np;
template <size_t N>
const size_t Tetrahedron<N>::nfp;
template <size_t N>
const size_t Tetrahedron<N>::nsc;

template<size_t N>
const Real Tetrahedron<N>::sizeFactor = (Real) (1.0 / 6.0);

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Tetrahedron.hpp"

#endif /* SEMBA_MATH_SIMPLEX_TETRAHEDRON_H_ */
