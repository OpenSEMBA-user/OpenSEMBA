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

#include "Line.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

template <size_t N>
Line<N>::Line() {

    buildNodeIndices();
    buildSideNodeIndices();

    lagrangePolynomials(lagr,N,np,nsc);
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t s = 0; s < nsc; s++) {
            dLagr[i][s] = lagr[i];
            dLagr[i][s].derive(s);
        }
    }

    buildCubaturePositionsAndWeights();
};

template <size_t N>
inline std::size_t Line<N>::nodeIndex(const std::size_t i,
                                      const std::size_t j) const {
    return nId[i](j);
}

template <size_t N>
inline std::size_t Line<N>::vertex(const std::size_t vertexNum) const {
    return sideNode(vertexNum,0);
}

template <size_t N>
inline std::size_t Line<N>::sideNode(const std::size_t face,
                                             const std::size_t num) const {
    return sNId(face, num);
}

template <size_t N>
const Function::Polynomial<Real>& Line<N>::getLagr(
        const std::size_t i) const {
    return lagr[i];
}

template <size_t N>
const Function::Polynomial<Real>& Line<N>::getDLagr(
        const std::size_t i,
        const std::size_t f) const {
    return dLagr[i][f];
}

template <size_t N>
void Line<N>::buildNodeIndices() {
    for (std::size_t i = 0; i < nId.size(); i++) {
        nId[i](0) = N - i;
        nId[i](1) = i;
    }
}

template <size_t N>
void Line<N>::buildSideNodeIndices() {
    Matrix::Static<Int,np,1> nList;
    for (std::size_t i = 0; i < np; i++) {
        nList(i,0) = i;
    }

    for (std::size_t f = 0; f < faces; f++) {
        Matrix::Static<Int,nfp,1> aux = RMatrix(f) * nList;
        for (std::size_t i = 0; i < nfp; i++) {
            sNId(f,i) = aux(i,0);
        }
    }
}

template <size_t N>
void Line<N>::buildCubaturePositionsAndWeights() {
    Vector::Cartesian<Real,nsc> aux;
    for (std::size_t i = 0; i < np; i++) {
        aux = nId[i];
        cPos[i] = aux / (Real) N;
    }
    for (std::size_t i = 0; i < np; i++) {
        weights[i] = integrate(lagr[i], dimension, sizeFactor) / sizeFactor;
    }
}

template <size_t N>
Matrix::Static<Int, Line<N>::np, Line<N>::np>
        Line<N>::PMatrix(const std::size_t s) const {
    Matrix::Static<Int,np,np> res;
    if (s == 0) {
        res.eye();
    } else {
        res.zeros();
        for (std::size_t i = 0; i < np; i++) {
            res(i, np-i-1) = (Int) 1;
        }
    }
    return res;
}

template <size_t N>
Matrix::Static<Int, Line<N>::nfp, Line<N>::np>
        Line<N>::RMatrix(const std::size_t s) const {

    Matrix::Static<Int,nfp,np> Raux;
    Raux.zeros();
    Raux(0,0) = (Int) 1;
    Matrix::Static<Int,np,np> P;
    P = PMatrix(s);
    Matrix::Static<Int,nfp,np> res = Raux * P;
    return res;
}

template <size_t N>
void Line<N>::printInfo() const {
    std::cout << " --- Line Information --- " << std::endl;
    std::cout << " Order:                         " << N << std::endl;
    std::cout << " Number of coordinates:         " << nsc << std::endl;
    std::cout << " Number of nodes:               " << np << std::endl;
    std::cout << " Number of face nodes:          " << nfp << std::endl;
    std::cout << " Rotation matrices:             " << std::endl;
    std::cout << " List of node indices:          " << std::endl;
    for (std::size_t i = 0; i < np; i++) {
        nId[i].printInfo();
        std::cout << std::endl;
    }
    std::cout << " List of side nodes indices:    " << std::endl;
    sNId.printInfo();
    std::cout << " Lagrange polynomials:          " << std::endl;
    for (std::size_t i = 0; i < np; i++) {
        std::cout << "Lagrange polynomial of node #" <<  i << std::endl;
        lagr[i].printInfo();
    }
    std::cout << " Lagrange polynomials derivatives: " << std::endl;
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t j = 0; j < faces; j++) {
            std::cout << "Pol. " << i << " derived w.r.t. var." 
                      << j << std::endl;
            dLagr[i][j].printInfo();
        }
    }
    std::cout << " Cubature positions and weights: " << std::endl;
    for (std::size_t i = 0; i < np; i++) {
        std::cout << "#" << i << " ";
        cPos[i].printInfo();
        std::cout << " " << weights[i] << std::endl;
    }
    std::cout << " --- End of simplex information --- " << std::endl;
}

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */
