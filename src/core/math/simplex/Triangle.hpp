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

#include "Triangle.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

// =============== Triangle methods =========================================
template <Int SIMPTRI_N>
Triangle<SIMPTRI_N>::Triangle() {
    for (UInt s = 0; s < nsc; s++) {
        P[s] = PMatrix(n,s);
    }
    buildNodeIndices(nId, n, np);
    for (UInt s = 0; s < nsc; s++) {
        R[s] = RMatrix(s);
    }
    buildSideNodeIndices();
    lagrangePolynomials(lagr,n,np,nsc);
    for (UInt i = 0; i < np; i++) {
        for (UInt s = 0; s < nsc; s++) {
            dLagr[i][s] = lagr[i];
            dLagr[i][s].derive(s);
        }
    }
    buildCubaturePositionsAndWeights();
    buildCubatureLagrange();
}


template <Int SIMPTRI_N>
void Triangle<SIMPTRI_N>::buildCubaturePositionsAndWeights() {
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

template <Int SIMPTRI_N>
Matrix::Static<Real, SIMPTRI_NP, SIMPTRI_NP>
        Triangle<SIMPTRI_N>::getMassMatrix() const {
    Matrix::Static<Real,np,np> res;
    for (UInt c = 0; c < ncp; c++) {
        res += cwaa[c];
    }
    return res;
}

template<Int SIMPTRI_N>
void Triangle<SIMPTRI_N>::buildCubatureLagrange() {
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

template <Int SIMPTRI_N>
inline UInt Triangle<SIMPTRI_N>::nodeIndex(const UInt i, const UInt j) const {
    return nId[i](j);
}

template <Int SIMPTRI_N>
inline UInt Triangle<SIMPTRI_N>::cubatureNodeIndex(UInt i, UInt j) const {
    return cId[i](j);
}

template <Int SIMPTRI_N>
const Function::Polynomial<Real>& Triangle<SIMPTRI_N>::getLagr(
        const UInt i) const {
    return lagr[i];
}

template <Int SIMPTRI_N>
const Function::Polynomial<Real>& Triangle<SIMPTRI_N>::getDLagr(
        const UInt i,
        const UInt f) const {
    return dLagr[i][f];
}

template<Int SIMPTRI_N>
inline UInt Triangle<SIMPTRI_N>::vertex(UInt i) const {
    return sideNode(i,0);
}

template<Int SIMPTRI_N>
inline UInt Triangle<SIMPTRI_N>::sideVertex(const UInt f, const UInt i) const {
    return sideNode(f,i);
}

template <Int SIMPTRI_N>
inline UInt Triangle<SIMPTRI_N>::sideNode(
const UInt face, const UInt num) const {
    return sNId(face, num);
}

template <Int SIMPTRI_N>
Vector::Cartesian<Real,3> Triangle<SIMPTRI_N>::coordinate(const UInt i) const {
    Vector::Cartesian<Real,3> res;
    res = nId[i];
    res /= (Real) n;
    return res;
}

template <Int SIMPTRI_N>
UInt Triangle<SIMPTRI_N>::numberOfNodes(const UInt order) const {
    Int res = 1;
    for (UInt i = 1; i < nsc; i++)
        res *= (order + i);
    res /= factorial(nsc-1);
    return res;
}

template <Int SIMPTRI_N>
void Triangle<SIMPTRI_N>::buildSideNodeIndices() {
    Matrix::Static<Int,np,1> nList;
    Matrix::Static<Int,nfp,1> aux;
    // Initializes node list.
    for (UInt i = 0; i < np; i++)
        nList(i,0) = i;
    // Creates aux matrix to store the computed sNId.
    for (UInt f = 0; f < faces; f++) {
        aux = R[f] * nList;
        for (UInt i = 0; i < nfp; i++)
            sNId(f,i) = aux(i,0);
    }
}

template <Int SIMPTRI_N>
Matrix::Dynamic<Int> Triangle<SIMPTRI_N>::PMatrix(UInt order, UInt s) const {
    // Creates indices rotation matrix for a simplex triangle element.
    // Rotation is clockwise.
    UInt np = numberOfNodes(order);
    UInt nfp = order + 1;
    Matrix::Dynamic<Int> res(np,np);
    Matrix::Dynamic<Int> original(nfp,nfp), rotated(nfp,nfp);
    // Creates original numeration matrix.
    UInt originalNum = 1;
    for (UInt i = 0; i < nfp; i++)
        for (UInt j = 0; j <= i; j++) {
            original(i,j) = originalNum;
            originalNum++;
        }
    // Creates rotated numeration matrix.
    UInt rotatedNum = 1;
    if (s == 0) {
        res.eye();
    } else if (s == 1 || s == 2) {
        for (UInt i = 0; i <= nfp; i++) {
            Int j = Int(nfp - 1);
            while (j >= Int(nfp - i)) {
                rotated(j,j-nfp+i) = rotatedNum;
                rotatedNum = rotatedNum + 1;
                j--;
            }
        }
        // Assigns values to P matrix.
        for (UInt i = 0; i < nfp; i++) {
            for (UInt j = 0; j <= i; j++) {
                res(rotated(i,j)-1,original(i,j)-1) = 1;
            }
        }
        if (s == 1)
            res = res * res;
    }
    return res;
}

template <Int SIMPTRI_N>
Matrix::Static<Int, SIMPTRI_NFP, SIMPTRI_NP> Triangle<SIMPTRI_N>::RMatrix(
        const UInt s) const {
    // Creates node indices vector with node indices on face 1.
    Matrix::Static<Int,nfp,1> nodeVec;
    UInt last = 0;
    for (UInt i = 0; i < n + 1; i++) {
        last += i;
        nodeVec(i,0) = last;
    }
    // Creates extraction matrix R for face 1.
    Matrix::Static<Int,nfp,np> Raux;
    for (UInt i = 0; i < nfp; i++)
        Raux(i,nodeVec(i,0)) = 1;
    // Returns final form of R.
    Matrix::Static<Int,nfp,np> res = Raux * P[s] * P[s];
    return res;
}

//template <Int SIMPTRI_N>
//Matrix::Static<Real, SIMPTRI_NP, SIMPTRI_NP>
//Triangle<SIMPTRI_N>::getMassMatrix() const {
//    vector<Matrix::Static<Real,np,np> > cwaa;
//    cwaa = cubatureLagrangeProducts();
//    Matrix::Static<Real,np,np> res;
//    for (UInt c = 0; c < cwaa.size(); c++) {
//        res += cwaa[c];
//    }
//    return res;
//}

//template <Int SIMPTRI_N> inline
//vector<vector<Real> >
//Triangle<SIMPTRI_N>::cubatureLagrange() const {
//    // PURPOSE:
//    // - Evaluates Lagrange and Lagrange derived polynomials in cubature
//    //   points.
//    TriangleRule triRule(3);
//    vector<Evaluation> ca(np, Evaluation(triRule.ncp));
//    for (UInt i = 0; i < np; i++)
//        ca[i] = triRule.eval(lagr[i]);
//    return ca;
//}
// 
//template <Int SIMPTRI_N> inline
//vector<Matrix::Static<Real, SIMPTRI_NP, SIMPTRI_NP> >
//Triangle<SIMPTRI_N>::cubatureLagrangeProducts() const {
//    // ========================================================================
//    // PURPOSE:
//    // - Gets cubatured alpha and derived alpha for simplex element
//    UInt i, j, c;
//    TriangleRule triRule(3);
//    Matrix::Static<Real, np, np> zero;
//    vector<Matrix::Static<Real, np, np> > res(triRule.ncp,  zero);
//    vector<Evaluation> ca(np, Evaluation(triRule.ncp));
//    ca = cubatureLagrange();
//    // Computes Cubature weighted alpha by alpha products.
//    for (c = 0; c < triRule.ncp; c++)
//        for (i = 0; i < np; i++)
//            for (j = 0; j < np; j++)
//                res[c](i,j) = triRule.w[c] * ca[i].val[c] * ca[j].val[c];
//    return res;
//}
// 
//template <Int SIMPTRI_N> inline
//void
//Triangle<SIMPTRI_N>::computeWeights() {
//    for (UInt i = 0; i < np; i++)
//        weight[i] = lagr[i].integrateInSimplexSpace();
//}

template <Int SIMPTRI_N>
void Triangle<SIMPTRI_N>::buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                                           const UInt order,
                                           const UInt nNodes) const {
    // Computes first coordinate indices vector.
    Matrix::Dynamic<Int> ini(nNodes,nsc);
    for (UInt i = 0; i <= order; i++)
        for (UInt j = numberOfNodes(UInt(i-1)); j < nNodes; j++)
            ini(j,0) = order - i;
    // Computes ordered nodal simplex coordinates.
    Matrix::Dynamic<Int> ord(nNodes,nsc);
    for (UInt i = 0; i < nsc; i++) {
        ord = PMatrix(order,i) * ini;
        // Stores in nId matrix.
        for (UInt j = 0; j < nNodes; j++) {
            res[j](i) = ord(j,0);
        }
    }
}

template <Int SIMPTRI_N>
void Triangle<SIMPTRI_N>::printInfo() const {
    std::cout << " --- Triangle Information --- "   << std::endl;
    std::cout << " Order:                         " <<   n << std::endl;
    std::cout << " Number of nodes:               " <<  np << std::endl;
    std::cout << " Number of face nodes:          " << nfp << std::endl;
    std::cout << " Number of coordinates:         " << nsc << std::endl;
    std::cout << " Rotation matrices:             "        << std::endl;
    for (UInt i = 0; i < nsc; i++) {
        std::cout << "Rotation around simplex coordinate #" << i << std::endl;
        P[i].printInfo();
    }
    std::cout << " List of node indices:          "        << std::endl;
    for (UInt i = 0; i < np; i++) {
        nId[i].printInfo();
        std::cout << std::endl;
    }
    std::cout << " Extraction matrices:           "        << std::endl;
    for (UInt i = 0; i < faces; i++) {
        std::cout << "Extraction matrices for face #" << i << std::endl;
        R[i].printInfo();
    }
    std::cout << " List of side nodes indices:    "        << std::endl;
    sNId.printInfo();
    std::cout << " Lagrange polynomials:          "        << std::endl;
    for (UInt i = 0; i < np; i++) {
        std::cout << "Lagrange polynomial of node #" <<  i << std::endl;
        lagr[i].printInfo();
    }
    std::cout << " Vertices:" << std::endl;
    for (UInt i = 0; i < vertices; i++)
        std::cout << "Vertex #" << i << ": " << vertex(i) << std::endl;
    //    std::cout << " Weights:                       "       << std::endl;
    //    Real totalWeight = 0.0;
    //    for (UInt i = 0; i < np; i++) {
    //        std::cout << weight[i] << std::endl;
    //        totalWeight += weight[i];
    //    }
    //    std::cout << " Weights sum: " << totalWeight          << std::endl;
    std::cout << " Order of cubature: " << nc << std::endl;
    std::cout << " Number of cub. points: " << ncp << std::endl;
    std::cout << " Cubature positions and weights: " << std::endl;
    for (UInt i = 0; i < ncp; i++) {
        std::cout << "#" << i << " ";
        cPos[i].printInfo();
        std::cout << " " << cw[i] << std::endl;
    }
    std::cout << " --- End of simplex information --- "   << std::endl;
}

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */
