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

// =============== Line =================================================
// =-=-=-=-=-=-=-= Constructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template <Int SIMPLIN_N>
Line<SIMPLIN_N>::Line() {
    // ---------- Matrices and indices ----------------------------------------
    /*for (Int s = 0; s < nsc; s++) {
        P[s] = PMatrix(n,s);
    }*/
    buildNodeIndices(nId, n, np);
    for (UInt s = 0; s < nsc; s++) {
        R[s] = RMatrix(s);
    }
    buildSideNodeIndices();
    // ----------- Lagrange polynomials ---------------------------------------
    lagrangePolynomials(lagr,n,np,nsc);
    for (UInt i = 0; i < np; i++) {
        for (UInt s = 0; s < nsc; s++) {
            dLagr[i][s] = lagr[i];
            dLagr[i][s].derive(s);
        }
    }
    // ----------- Cubature positions and weights -----------------------------
    buildCubaturePositionsAndWeights();
    // ----------- Cubature lagranges -----------------------------------------
    buildCubatureLagrange();
};

template <Int SIMPLIN_N>
inline UInt Line<SIMPLIN_N>::nodeIndex(const UInt i, const UInt j) const {
    return nId[i](j);
}

template <Int SIMPLIN_N>
inline UInt Line<SIMPLIN_N>::cubatureNodeIndex(UInt i, UInt j) const {
    return cId[i](j);
}

template <Int SIMPLIN_N>
inline UInt Line<SIMPLIN_N>::vertex(const UInt vertexNum) const {
    return sideNode(vertexNum,0);
}

template <Int SIMPLIN_N>
inline UInt Line<SIMPLIN_N>::sideNode(const UInt face, const UInt num) const {
    return sNId(face, num);
}

template <Int SIMPLIN_N>
const Function::Polynomial<Real>& Line<SIMPLIN_N>::getLagr(
        const UInt i) const {
    return lagr[i];
}

template <Int SIMPLIN_N>
const Function::Polynomial<Real>& Line<SIMPLIN_N>::getDLagr(
        const UInt i,
        const UInt f) const {
    return dLagr[i][f];
}

template <Int SIMPLIN_N>
void Line<SIMPLIN_N>::buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                                       const UInt order,
                                       const UInt nNodes) const {
    // Computes first coordinate indices vector.
    for (UInt i = 0; i < nNodes; i++) {
        res[i](0) = order - i;
        res[i](1) = i;
    }
}

template <Int SIMPLIN_N>
void Line<SIMPLIN_N>::buildSideNodeIndices() {
    Matrix::Static<Int,np,1> nList;
    Matrix::Static<Int,nfp,1> aux;
    // Initializes node list.
    for (UInt i = 0; i < np; i++)
        nList(i,0) = i;
    // Creates aux matrix to store the computed sNId.
    for (UInt f = 0; f < faces; f++) {
        aux = R[f] * nList;
        for (UInt i = 0; i < nfp; i++) {
            sNId(f,i) = aux(i,0);
        }
    }
}

template <Int SIMPLIN_N>
void Line<SIMPLIN_N>::buildCubaturePositionsAndWeights() {
    buildNodeIndices(cId,nc,ncp);
    Vector::Cartesian<Real,nsc> aux;
    for (UInt i = 0; i < ncp; i++) {
        aux = cId[i];
        cPos[i] = aux / (Real) nc;
    }
    Function::Polynomial<Real> cubLagr[ncp];
    cubatureLagrangePolynomials(cubLagr,nc,ncp,nsc);
    for (UInt i = 0; i < ncp; i++) {
        cw[i] = integrate(cubLagr[i], dimension, sizeFactor) / sizeFactor;
    }
}

template<Int SIMPLIN_N>
void Line<SIMPLIN_N>::buildCubatureLagrange() {
    UInt i, j, k, c;
    // Evaluates Lagrange and Lagrange derived polynomials in cubature points.
    for (UInt i = 0; i < np; i++) {
        for (UInt c = 0; c < ncp; c++) {
            ca[i][c] = lagr[i].eval(cPos[c]);
        }
    }
    for (UInt i = 0; i < np; i++) {
        for (UInt j = 0; j < faces; j++) {
            for (UInt c = 0; c < ncp; c++) {
                cda[i][j][c] = dLagr[i][j].eval(cPos[c]);
            }
        }
    }
    // Computes Cubature weighted alpha by alpha products.
    for (c = 0; c < ncp; c++) {
        for (i = 0; i < np; i++)
            for (j = 0; j < np; j++)
                cwaa[c](i,j) = cw[c] * ca[i][c] * ca[j][c];
        // Computes Cubature weighted alpha by alpha derivatives products.
        for (k = 0; k < faces; k++)
            for (i = 0; i < np; i++)
                for (j = 0; j < np; j++)
                    cwada[c][k](i,j) = cw[c] * ca[i][c] * cda[j][k][c];
    }
}

template<Int SIMPLIN_N>
UInt Line<SIMPLIN_N>::numberOfNodes(const UInt order) const {
    return (order + 1);
}

template <Int SIMPLIN_N>
Matrix::Dynamic<Int> Line<SIMPLIN_N>::PMatrix(const UInt n,
                                              const UInt s) const {
    UInt np = numberOfNodes(n);
    Matrix::Dynamic<Int> res(np,np);
    if (s == 0) {
        res.eye();
    } else  {
        res.zeros();
        for (UInt i = 0; i < np; i++) {
            res(i, np-i-1) = (Int) 1;
        }
    }
    return res;
}

template <Int SIMPLIN_N>
Matrix::Static<Int,SIMPLIN_NFP,SIMPLIN_NP> Line<SIMPLIN_N>::RMatrix(
        const UInt s) const {
    // Creates extraction matrix R for face 1.
    Matrix::Static<Int,nfp,np> Raux;
    Raux.zeros();
    Raux(0,0) = (Int) 1;
    Matrix::Static<Int,nfp,np> res = Raux * P[s];
    return res;
}

template <Int SIMPLIN_N>
void Line<SIMPLIN_N>::printInfo() const {
    std::cout << " --- Line Information --- " << std::endl;
    std::cout << " Number of coordinates:         " << nsc << std::endl;
    std::cout << " Order:                         " << n << std::endl;
    std::cout << " Number of nodes:               " << np << std::endl;
    std::cout << " Number of face nodes:          " << nfp << std::endl;
    std::cout << " Order of cubature integration: " << nc << std::endl;
    std::cout << " Rotation matrices:             " << std::endl;
    for (UInt i = 0; i < faces; i++) {
        std::cout << "Rotation around simplex coordinate #" << i << std::endl;
        P[i].printInfo();
    }
    std::cout << " Extraction matrices:           " << std::endl;
    for (UInt i = 0; i < faces; i++) {
        std::cout << "Extraction matrices for face #" << i << std::endl;
        R[i].printInfo();
    }
    std::cout << " List of node indices:          " << std::endl;
    for (UInt i = 0; i < np; i++) {
        nId[i].printInfo();
        std::cout << std::endl;
    }
    std::cout << " List of side nodes indices:    " << std::endl;
    sNId.printInfo();
    std::cout << " Lagrange polynomials:          " << std::endl;
    for (UInt i = 0; i < np; i++) {
        std::cout << "Lagrange polynomial of node #" <<  i << std::endl;
        lagr[i].printInfo();
    }
    std::cout << " Lagrange polynomials derivatives: " << std::endl;
    for (UInt i = 0; i < np; i++) {
        for (UInt j = 0; j < faces; j++) {
            std::cout << "Pol. " << i << " derived w.r.t. var." 
                      << j << std::endl;
            dLagr[i][j].printInfo();
        }
    }
    std::cout << " Cubature positions and weights: " << std::endl;
    for (UInt i = 0; i < ncp; i++) {
        std::cout << "#" << i << " ";
        cPos[i].printInfo();
        std::cout << " " << cw[i] << std::endl;
    }
    std::cout << " --- End of simplex information --- " << std::endl;
}

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */
