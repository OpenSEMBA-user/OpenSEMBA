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
    buildNodeIndices(nId, N, np);
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
inline std::size_t Line<N>::nodeIndex(
        const std::size_t i, const std::size_t j) const {
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
void Line<N>::buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
        const std::size_t order,
        const std::size_t nNodes) const {
    for (std::size_t i = 0; i < nNodes; i++) {
        res[i](0) = order - i;
        res[i](1) = i;
    }
}

template <size_t N>
void Line<N>::buildSideNodeIndices() {
    // Initializes node list.
    Matrix::Static<Int,np,1> nList;
    for (std::size_t i = 0; i < np; i++) {
        nList(i,0) = i;
    }
    // Creates aux matrix to store the computed sNId.
    for (std::size_t f = 0; f < nsc; f++) {
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
        cw[i] = integrate(lagr[i], dimension, sizeFactor_) / sizeFactor_;
    }
}

template<size_t N>
void Line<N>::buildCubedLagrange_() {
    // Evaluates Lagrange and Lagrange derived polynomials in cubature points.
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t c = 0; c < np; c++) {
            ca[i][c] = lagr[i].eval(cPos[c]);
        }
    }
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t j = 0; j < nsc; j++) {
            for (std::size_t c = 0; c < np; c++) {
                cda[i][j][c] = dLagr[i][j].eval(cPos[c]);
            }
        }
    }
    // Computes Cubature weighted alpha by alpha products.
    for (size_t c = 0; c < np; c++) {
        for (size_t i = 0; i < np; i++)
            for (size_t j = 0; j < np; j++)
                cwaa[c](i,j) = cw[c] * ca[i][c] * ca[j][c];
        // Computes Cubature weighted alpha by alpha derivatives products.
        for (size_t k = 0; k < nsc; k++)
            for (size_t i = 0; i < np; i++)
                for (size_t j = 0; j < np; j++)
                    cwada[c][k](i,j) = cw[c] * ca[i][c] * cda[j][k][c];
    }
}

template<size_t N>
std::size_t Line<N>::numberOfNodes_(size_t order) {
    return order + 1;
}

template <size_t N>
Matrix::Static<Int,Line<N>::np,Line<N>::np> Line<N>::PMatrix(
        const std::size_t s) const {
    Matrix::Static<Int,np,np> res;
    res = PMatrix_(N,s);
    return res;
}

template <size_t N>
Matrix::Dynamic<Int> Line<N>::PMatrix_(
        const std::size_t n,
        const std::size_t s) const {
    std::size_t np = numberOfNodes_(n);
    Matrix::Dynamic<Int> res(np,np);
    if (s == 0) {
        res.eye();
    } else  {
        res.zeros();
        for (std::size_t i = 0; i < np; i++) {
            res(i, np-i-1) = (Int) 1;
        }
    }
    return res;
}

template <size_t N>
Matrix::Static<Int,Line<N>::nfp,Line<N>::np> Line<N>::RMatrix(
        const std::size_t s) const {
    // Creates extraction matrix R for face 1.
    Matrix::Static<Int,nfp,np> Raux;
    Raux.zeros();
    Raux(0,0) = (Int) 1;
    Matrix::Static<Int,np,np> P = PMatrix(s);
    Matrix::Static<Int,nfp,np> res = Raux * P;
    return res;
}

template <size_t N>
void Line<N>::printInfo() const {
    std::cout << " --- Simplex::Line Info --- " << std::endl;
    Simplex::printInfo();
    std::cout << " Rotation matrices:             " << std::endl;
    for (std::size_t i = 0; i < nsc; i++) {
        std::cout << "Rotation around simplex coordinate #" << i << std::endl;
        PMatrix(i).printInfo();
    }
    std::cout << " Extraction matrices:           " << std::endl;
    for (std::size_t i = 0; i < nsc; i++) {
        std::cout << "Extraction matrices for face #" << i << std::endl;
        RMatrix(i).printInfo();
    }
    std::cout << " --- End of Simplex::Line info --- " << std::endl;
}

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */
