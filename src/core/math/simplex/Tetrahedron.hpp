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

    buildSideNodeIndices();
    lagrangePolynomials(lagr,n,np,nsc);
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t s = 0; s < nsc; s++) {
            dLagr[i][s] = lagr[i];
            dLagr[i][s].derive(s);
        }
    }
    buildCubaturePositionsAndWeights();
    buildCubedLagrange_();
    buildLIFTMatrix();
}

template <size_t N>
void Tetrahedron<N>::buildCubaturePositionsAndWeights() {
    for (std::size_t i = 0; i < np; i++) {
        cPos[i] = nId[i] / (Real) n;
    }
    for (std::size_t i = 0; i < np; i++) {
        cw[i] = integrate(lagr[i], dimension, sizeFactor) / sizeFactor;
    }
}

template <size_t N>
void Tetrahedron<N>::buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
        const std::size_t order,
        const std::size_t nNodes) const {
    // Computes first coordinate indices vector.
    Matrix::Dynamic<Int> ini(nNodes,nsc);
    for (std::size_t i = 0; i <= order; i++) {
        for (std::size_t j = numberOfNodes(i-1); j < nNodes; j++) {
            ini(j,0) = order - i;
        }
    }
    // Computes ordered nodal simplex coordinates.
    Matrix::Dynamic<Int> ord(nNodes,nsc);
    for (std::size_t i = 0; i < nsc; i++) {
        ord = PMatrix_(order,i) * ini;
        // Stores in nId matrix.
        for (std::size_t j = 0; j < nNodes; j++) {
            res[j](i) = ord(j,0);
        }
    }
}


template<size_t N>
void Tetrahedron<N>::buildCubedLagrange_() {
    std::size_t i, j, k, c;
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
    for (c = 0; c < np; c++) {
        for (i = 0; i < np; i++)
            for (j = 0; j < np; j++)
                cwaa[c](i,j) = cw[c] * ca[i][c] * ca[j][c];
        // Computes Cubature weighted alpha by alpha derivatives products.
        for (k = 0; k < nsc; k++)
            for (i = 0; i < np; i++)
                for (j = 0; j < np; j++)
                    cwada[c][k](i,j) = cw[c] * ca[i][c] * cda[j][k][c];
    }
}

template <size_t N>
inline std::size_t Tetrahedron<N>::nodeIndex(
        const std::size_t i,
        const std::size_t j) const {
    return nId[i](j);
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
Real Tetrahedron<N>::getCda(std::size_t i,
        std::size_t j,
        std::size_t k) const {
    return cda[i][j][k];
}

template <size_t N>
Matrix::Static<Real, Tetrahedron<N>::np, Tetrahedron<N>::np>
Tetrahedron<N>::getMassMatrix() const {
    Matrix::Static<Real,np,np> res;
    for (std::size_t c = 0; c < np; c++) {
        res += cwaa[c];
    }
    return res;
}

template <size_t N>
void Tetrahedron<N>::buildLIFTMatrix() {
    // Obtains mass matrix for the tetrahedron.
    Matrix::Static<Real,np,np> invM;
    invM = getMassMatrix().invert();
    // Obtains side mass matrix using simplex triangle.
    Matrix::Static<Real, nfp, nfp> sM;
    sM = tri.getMassMatrix();
    // Computes LIFT matrix.
    Matrix::Static<Int, np, nfp> RtrInt;
    Matrix::Static<Real, np, nfp> RtrDbl;
    for (std::size_t f = 0; f < nsc; f++) {
        RtrInt = RMatrix(f).transpose();
        RtrDbl.assign(RtrInt);
        LIFT[f] = RtrDbl * sM;
        LIFT[f] = invM * LIFT[f];
    }
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
Real Tetrahedron<N>::integrateScalarsOnFace(
        const Real x[Tetrahedron<N>::np],
        const std::size_t f,
        const Real area) const {
    // Takes an array of np scalars, a face number f and an area.
    // Returns a vector that is the result of integrating over the face of
    // the simplex and multiply the result by the area.
    Real faceX[nfp];
    for (std::size_t i = 0; i < nfp; i++)
        faceX[i] = x[sideNode(f,i)];
    // Computes result.
    Real res;
    for (std::size_t i = 0; i < nfp; i++) {
        for (std::size_t c = 0; c < np; c++) {
            res += tri.cw[c] * faceX[i];
        }
    }
    res *= area;
    throw std::logic_error(
            "Tetrahedron::integrateScalarsOnFace not implemented");
    return res;
}

template <size_t N>
Real Tetrahedron<N>::integrateScalars(const Real x[Tetrahedron<N>::np],
        const Real volume) const {
    Real res = 0.0;
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t c = 0; c < np; c++) {
            res += cw[c] * ca[i][c] * x[i];
        }
    }
    return (res*volume);
}

template <size_t N>
Vector::Cartesian<Real,4> Tetrahedron<N>::coordinate(
        const std::size_t i) const {
    Vector::Cartesian<Real,4> res;
    res = nId[i];
    res /= (Real) n;
    return res;
}

template <size_t N>
Vector::Cartesian<Real,4> Tetrahedron<N>::sideCoordinate(
        const std::size_t f,
        const std::size_t i) const {
    return coordinate(sideNode(f,i));
}

template <size_t N>
void Tetrahedron<N>::buildSideNodeIndices() {
    // Initializes node list.
    Matrix::Static<Int,np,1> nList;
    for (std::size_t i = 0; i < np; i++) {
        nList(i,0) = i;
    }
    // Creates aux matrix to store the computed sNId.
    for (std::size_t f = 0; f < nsc; f++) {
        Matrix::Static<Int,nfp,1> aux = RMatrix(f) * nList;
        for (std::size_t i = 0; i < nfp; i++)
            sNId(f,i) = aux(i,0);
    }
}

template<size_t N>
size_t Tetrahedron<N>::numberOfNodes(size_t order) {
    Int res = 1;
    for (Int i = 1; i < Int(nsc); i++) {
        res *= (order + i);
    }
    res /= factorial(nsc - 1);
    return res;
}

template <size_t N>
Matrix::Dynamic<Int> Tetrahedron<N>::PMatrix_(
        const std::size_t n,
        std::size_t s) const {
    // Purpose: Creates indices rotation matrix for a simplex tetrahedral
    //          element in 3-D.
    std::size_t np = numberOfNodes(n);
    Matrix::Dynamic<Int> res(np,np);
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
        Matrix::Dynamic<Int> Q[nsc];
        Matrix::Dynamic<Int> QInit(np,np);
        for (std::size_t i = 0; i < nsc; i++) {
            Q[i] = QInit;
        }
        for (std::size_t i = 0; i < np; i++) {
            Q[0](orVec(i,0),fiVec(i,0)) = 1;
        }
        // Defines Q[1], same as Q[0] but around vertex 1.
        // Assigns nodes to the original (non-rotated) enumeration vector.
        orVec.zeros();
        std::size_t lastSet = n * (n+1) * (n+2) / 6 + 1;
        std::size_t lastNodeSet = 0;
        for (std::size_t i = 0; i <= n; i++)
            for (std::size_t j = 1; j <= i+1; j++) {
                orVec(lastNodeSet+j-1,0) = lastSet;
                lastSet += 1;
                lastNodeSet += j;
            }
        lastSet = n * (n+1) * (n+2) / 6 + 1;
        for (std::size_t j = 1; j <= n; j++) {
            temp = orVec;
            for(Int i = Int(np); i >= 2; i--) {
                if (temp(i-1,0) != 0)
                    if (temp(i-2,0) == 0) {
                        lastSet--;
                        orVec(i-2,0) = lastSet;
                    }
            }
        }
        // Creates final (rotated) enumeration vector.
        fiVec = P2Dall * orVec;
        // Assigns values to Q[1].
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
Matrix::Static<Int,Tetrahedron<N>::nfp,Tetrahedron<N>::np>
Tetrahedron<N>::RMatrix(const std::size_t s) const {
    // Creates node indices vector with node indices on face 1.
    std::size_t last = 0;
    Matrix::Static<Int,nfp,1> nodeVec;
    for (std::size_t i = 1; i <= n + 1; i++) {
        std::size_t nsp = (i-1) * (i) * (i+1) / 6;
        for (std::size_t j = 0; j<i; j++) {
            nodeVec(last + j,0) = nsp + (j)*(j+1)/2 + 1;
        }
        last += i;
    }
    // Creates extraction matrix R for face 1.
    Matrix::Static<Int,nfp,np> Raux;
    for (std::size_t i = 0; i < nfp; i++) {
        Raux(i,nodeVec(i,0)-1) = 1;
    }
    // Returns final form of R.
    Matrix::Static<Int,np,np> P;
    P = PMatrix_(N,s);
    Matrix::Static<Int,nfp,np> res = Raux * P;
    return res;
}

template <size_t N>
inline Real Tetrahedron<N>::getWeight(const std::size_t i) const {
    return cw[i];
}

template <size_t N>
void Tetrahedron<N>::printInfo() const {
    std::cout << " --- Simplex::Tetrahedron info --- " << std::endl;
    Simplex::printInfo();
    std::cout << " Rotation matrices:             " << std::endl;
    for (std::size_t i = 0; i < nsc; i++) {
        std::cout << "Rotation around simplex coordinate #" << i << std::endl;
        PMatrix_(N,i).printInfo();
    }
    std::cout << " Extraction matrices:           " << std::endl;
    for (std::size_t i = 0; i < nsc; i++) {
        std::cout << "Extraction matrices for face #" << i << std::endl;
        RMatrix(i).printInfo();
    }
    std::cout << " --- End of Simplex::Tetrahedron info --- "   << std::endl;
}

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */
