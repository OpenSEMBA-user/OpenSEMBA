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

#ifndef SEMBA_MATH_SIMPLEX_LINE_H_
#define SEMBA_MATH_SIMPLEX_LINE_H_

#include "math/matrix/Static.h"

#include "Simplex.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

template <size_t N>
class Line : public Simplex {
public:
    static const std::size_t n;
    static const std::size_t np;
    static const std::size_t nfp;
    static const std::size_t nsc;

    Line();

    std::size_t order() const {return n;}
    std::size_t numberOfSimplexCoordinates() {return nsc;}
    std::size_t numberOfNodes() {return np;}
    std::size_t numberOfFaceNodes() {return nfp;}

    std::size_t vertex(const std::size_t) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t sideNode(const std::size_t f, const std::size_t i) const;
    std::size_t nodeIndex(const std::size_t i, const std::size_t j) const;

    const Function::Polynomial<Real>& getLagr(const std::size_t i) const;
    const Function::Polynomial<Real>& getDLagr(const std::size_t i,
                                               const std::size_t f) const;
    void printInfo() const;

private:
    static const std::size_t dimension = 1;

    Vector::Cartesian<Int,nsc> nId[np];
    Matrix::Static<Int,nsc,nfp> sNId;
    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][nsc];

    static const Real sizeFactor_;

    Vector::Cartesian<Real,nsc> cPos[np];
    Real cw[np];
    Real ca[np][np];
    Real cda[np][nsc][np];
    Matrix::Static<Real,np,np> cwaa[np];
    Matrix::Static<Real,np,np> cwada[np][nsc];

    static Matrix::Static<Int,np,np> PMatrix(const std::size_t s);
    static Matrix::Static<Int,nfp,np> RMatrix(const std::size_t s);

    static Matrix::Dynamic<Int> PMatrix_(const std::size_t n,
                                 const std::size_t s);

    void buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                          const std::size_t order,
                          const std::size_t nNodes) const;
    void buildSideNodeIndices();

    static std::size_t numberOfNodes_(size_t order);

    void buildCubaturePositionsAndWeights();
    void buildCubedLagrange_();
};

template <size_t N>
const size_t Line<N>::n = N;

template <size_t N>
const size_t Line<N>::np = N + 1;

template <size_t N>
const size_t Line<N>::nfp = 1;

template <size_t N>
const size_t Line<N>::nsc = 2;

template <size_t N>
const Real Line<N>::sizeFactor_ = 1.0;

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Line.hpp"

#endif /* SEMBA_MATH_SIMPLEX_LINE_H_ */
