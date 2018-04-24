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
#include "Tetrahedron.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

template <size_t N>
Tetrahedron<N>::Tetrahedron() {

    Matrix::Static<Int,np,nsc> ini;
    for (std::size_t i = 0; i <= N; i++) {
        for (std::size_t j = numberOfNodes(i-1); j < np; j++) {
            ini(j,0) = N - i;
        }
    }

    Matrix::Static<Int,np,nsc> ord;
    for (std::size_t i = 0; i < nsc; i++) {
        ord = PMatrix(i) * ini;
        for (std::size_t j = 0; j < np; j++) {
            indices[j](i) = ord(j,0);
        }
    }

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

    lagrangePolynomials(lagr,n,np,nsc);
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t s = 0; s < nsc; s++) {
            dLagr[i][s] = lagr[i];
            dLagr[i][s].derive(s);
        }
    }

    for (std::size_t i = 0; i < np; i++) {
        Vector::Cartesian<Real,nsc> aux;
        nodePositions[i] = aux / (Real) N;
    }
    for (std::size_t i = 0; i < np; i++) {
        weights[i] = integrate(lagr[i], dimension, sizeFactor) / sizeFactor;
    }
}

template <size_t N>
inline std::vector<Real> Tetrahedron<N>::getWeights() const {
    std::vector<Real> res(np);
    std::copy_n(weights.begin(), np, res.begin());
    return res;
}

template <size_t N>
inline const Function::Polynomial<Real>& Tetrahedron<N>::getLagr(
        const std::size_t i) const {
    return lagr[i];
}

template <size_t N>
inline const Function::Polynomial<Real>& Tetrahedron<N>::getDLagr(
        const std::size_t i,
        const std::size_t f) const {
    return dLagr[i][f];
}

template <size_t N>
inline std::size_t Tetrahedron<N>::vertex(
        const std::size_t vertexNum) const {
    return sideNode(vertexNum,0);
}

template <size_t N>
inline std::size_t Tetrahedron<N>::sideVertex(
        const std::size_t face,
        const std::size_t vertexNum) const {
    return sideNode(face, tri.vertex(vertexNum));
}

template <size_t N>
inline std::size_t Tetrahedron<N>::sideNode(
        const std::size_t face,
        const std::size_t num) const {
    return sNId(face, num);
}

template <size_t N>
inline std::size_t Tetrahedron<N>::nodeIndex(const std::size_t i,
                                          const std::size_t j) const {
    return indices[i](j);
}

template <size_t N>
Vector::Cartesian<Real,4> Tetrahedron<N>::coordinate(
        const std::size_t i) const {
    Vector::Cartesian<Real,4> res = indices[i];
    res /= (Real) n;
    return res;
}

template <size_t N>
Vector::Cartesian<Real,4> Tetrahedron<N>::sideCoordinate(
        const std::size_t f,
        const std::size_t i) const {
    return coordinate(sideNode(f,i));
}

template<size_t N>
size_t Tetrahedron<N>::numberOfNodes(size_t order) {
    size_t res = 1;
    for (size_t i = 1; i < size_t (nsc); i++) {
        res *= (order + i);
    }
    res /= factorial(nsc - 1);
    return res;
}

template <size_t N>
Matrix::Static<Int, TET_NP, TET_NP>
        Tetrahedron<N>::PMatrix(std::size_t s) const {
    Matrix::Static<Int,np,np> res;
    if (s == 0) {
        res.eye();
    } else if (s >= 1 && s <= 3) {
        // P2Dall will store the rotation matrix for all the indices slices.
        Matrix::Dynamic<Int> P2Dall(np,np);
        std::size_t nodesSet = 0;
        for (std::size_t i = 0; i <= n; i++) {
            std::size_t sliceNP = (i + 1) * (i + 2) / 2;
            // Assign P2D to P2Dall matrix.
            Matrix::Dynamic<Int> auxP = tri.PMatrix(i,2);
            for (std::size_t j = 0; j < sliceNP; j++) {
                for (std::size_t k = 0; k < sliceNP; k++) {
                    P2Dall(j+nodesSet,k+nodesSet) = auxP.val(j,k);
                }
            }
            nodesSet += sliceNP;
        }
        // Allocates and initializes ordered and final indices vectors.
        Matrix::Dynamic<Int> orVec(np,1), fiVec(np,1), temp(np,1);
        for (std::size_t i = 0; i < np; i++) {
            orVec(i,0) = i;
        }
        // Computes final indices positions.
        fiVec = P2Dall * orVec;
        // Assigns ones to the Q1 matrix.
        Matrix::Dynamic<Int> Q[faces];
        Matrix::Dynamic<Int> QInit(np,np);
        for (std::size_t i = 0; i < faces; i++) {
            Q[i] = QInit;
        }
        for (std::size_t i = 0; i < np; i++) {
            Q[0](orVec(i,0),fiVec(i,0)) = 1;
        }

        // Defines Q[1], same as Q[0] but around vertex 1.
        orVec.zeros();
        std::size_t lastSet = n * (n+1) * (n+2) / 6 + 1;
        std::size_t lastNodeSet = 0;
        for (std::size_t i = 0; i <= n; i++) {
            for (std::size_t j = 1; j <= i+1; j++) {
                orVec(lastNodeSet+j-1,0) = lastSet;
                lastSet += 1;
                lastNodeSet += j;
            }
        }
        lastSet = n * (n+1) * (n+2) / 6 + 1;
        for (std::size_t j = 1; j <= n; j++) {
            temp = orVec;
            for(Int i = Int(np); i >= 2; i--) {
                if (temp(i-1,0) != 0) {
                    if (temp(i-2,0) == 0) {
                        lastSet--;
                        orVec(i-2,0) = lastSet;
                    }
                }
            }
        }

        fiVec = P2Dall * orVec;
        for (std::size_t i = 0; i < np; i++) {
            Q[1](orVec(i,0)-1,fiVec(i,0)-1) = 1;
        }
        Matrix::Dynamic<Int> Q0Sq, Q1Sq;
        Q0Sq = Q[0] * Q[0];
        Q1Sq = Q[1] * Q[1];
        switch (s) {
        case 1:
            Q[2] = Q1Sq * Q0Sq; // Q[2] = Q[1] * Q[1] * Q[0] * Q[0];
            res = Q[2] * Q1Sq;  // res = Q[2] * Q[1] * Q[1];
            break;
        case 2:
            Q[2] = Q1Sq * Q0Sq; // Q[2] = Q[1] * Q[1] * Q[0] * Q[0];
            Q[3] = Q0Sq * Q1Sq; // Q[3] = Q[0] * Q[0] * Q[1] * Q[1];
            res = Q[3] * Q[2] * Q[2];
            break;
        case 3:
            Q[3] = Q0Sq * Q1Sq;// Q[3] = Q[0] * Q[0] * Q[1] * Q[1];
            res = Q[1] * Q[3] * Q[3];
            break;
        }
    }
    return res;
}

template <size_t N>
Matrix::Static<Int, TET_NFP, TET_NP>
        Tetrahedron<N>::RMatrix(const std::size_t s) const {
    std::size_t last = 0;
    Matrix::Static<Int,nfp,1> nodeVec;
    for (std::size_t i = 1; i <= n + 1; i++) {
        std::size_t nsp = (i-1) * (i) * (i+1) / 6;
        for (std::size_t j = 0; j<i; j++) {
            nodeVec(last + j,0) = nsp + (j)*(j+1)/2 + 1;
        }
        last += i;
    }

    Matrix::Static<Int,nfp,np> Raux;
    for (std::size_t i = 0; i < nfp; i++) {
        Raux(i,nodeVec(i,0)-1) = 1;
    }

    return Raux * PMatrix(s);
}

template <size_t N>
void Tetrahedron<N>::printInfo() const {
    std::cout << " --- Tetrahedron Information --- " << std::endl;
    std::cout << " Order:                         " << n << std::endl;
    std::cout << " List of node indices: " << std::endl;
    for (std::size_t i = 0; i < np; i++) {
        indices[i].printInfo();
        std::cout << std::endl;
    }
    std::cout << " List of side nodes indices:    " << std::endl;
    sNId.printInfo();
    std::cout << " List of side nodes coordinates: " << std::endl;
    for (std::size_t f = 0; f < faces; f++) {
        std::cout << "Face #" << f << std::endl;
        for (std::size_t i = 0; i < nfp; i++) {
            sideCoordinate(f,i).printInfo();
            std::cout << std::endl;
        }
    }
    std::cout << " --- End of simplex information --- "   << std::endl;
}


} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */
