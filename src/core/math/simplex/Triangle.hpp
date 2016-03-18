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

template <size_t N>
Triangle<N>::Triangle() {
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
}


template <size_t N>
void Triangle<N>::buildCubaturePositionsAndWeights() {
    for (std::size_t i = 0; i < np; i++) {
        cPos[i] = nId[i] / (Real) n;
    }
    for (std::size_t i = 0; i < np; i++) {
        cw[i] = integrate(lagr[i], dimension, sizeFactor_) / sizeFactor_;
    }
}

template <size_t N>
Matrix::Static<Real, Triangle<N>::np, Triangle<N>::np>
        Triangle<N>::getMassMatrix() const {
    Matrix::Static<Real,np,np> res;
    for (std::size_t c = 0; c < np; c++) {
        res += cwaa[c];
    }
    return res;
}

template<size_t N>
void Triangle<N>::buildCubedLagrange_() {
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
inline std::size_t Triangle<N>::nodeIndex(const std::size_t i,
                                                  const std::size_t j) const {
    return nId[i](j);
}

template <size_t N>
const Function::Polynomial<Real>& Triangle<N>::getLagr(
        const std::size_t i) const {
    return lagr[i];
}

template <size_t N>
const Function::Polynomial<Real>& Triangle<N>::getDLagr(
        const std::size_t i,
        const std::size_t f) const {
    return dLagr[i][f];
}

template <size_t N>
inline Real Triangle<N>::getWeight(const std::size_t i) const {
    return cw[i];
}

template<size_t N>
inline std::size_t Triangle<N>::vertex(std::size_t i) const {
    return sideNode(i,0);
}

template<size_t N>
inline std::size_t Triangle<N>::sideVertex(const std::size_t f,
                                                   const std::size_t i) const {
    return sideNode(f,i);
}

template <size_t N>
inline std::size_t Triangle<N>::sideNode(
const std::size_t face, const std::size_t num) const {
    return sNId(face, num);
}

template <size_t N>
Vector::Cartesian<Real,3> Triangle<N>::coordinate(
        const std::size_t i) const {
    Vector::Cartesian<Real,3> res;
    res = nId[i];
    res /= (Real) n;
    return res;
}

template <size_t N>
std::size_t Triangle<N>::numberOfNodes(const std::size_t order) const {
    Int res = 1;
    for (std::size_t i = 1; i < nsc; i++)
        res *= (order + i);
    res /= factorial(nsc-1);
    return res;
}

template <size_t N>
void Triangle<N>::buildSideNodeIndices() {
    Matrix::Static<Int,np,1> nList;
    Matrix::Static<Int,nfp,1> aux;
    // Initializes node list.
    for (std::size_t i = 0; i < np; i++)
        nList(i,0) = i;
    // Creates aux matrix to store the computed sNId.
    for (std::size_t f = 0; f < nsc; f++) {
        aux = RMatrix(f) * nList;
        for (std::size_t i = 0; i < nfp; i++)
            sNId(f,i) = aux(i,0);
    }
}

template <size_t N>
Matrix::Dynamic<Int> Triangle<N>::PMatrix(
        std::size_t order,
        std::size_t s) const {
    // Creates indices rotation matrix for a simplex triangle element.
    // Rotation is clockwise.
    std::size_t np = numberOfNodes(order);
    std::size_t nfp = order + 1;
    Matrix::Dynamic<Int> res(np,np);
    Matrix::Dynamic<Int> original(nfp,nfp), rotated(nfp,nfp);
    // Creates original numeration matrix.
    std::size_t originalNum = 1;
    for (std::size_t i = 0; i < nfp; i++)
        for (std::size_t j = 0; j <= i; j++) {
            original(i,j) = originalNum;
            originalNum++;
        }
    // Creates rotated numeration matrix.
    std::size_t rotatedNum = 1;
    if (s == 0) {
        res.eye();
    } else if (s == 1 || s == 2) {
        for (std::size_t i = 0; i <= nfp; i++) {
            Int j = Int(nfp - 1);
            while (j >= Int(nfp - i)) {
                rotated(j,j-nfp+i) = rotatedNum;
                rotatedNum = rotatedNum + 1;
                j--;
            }
        }
        // Assigns values to P matrix.
        for (std::size_t i = 0; i < nfp; i++) {
            for (std::size_t j = 0; j <= i; j++) {
                res(rotated(i,j)-1,original(i,j)-1) = 1;
            }
        }
        if (s == 1)
            res = res * res;
    }
    return res;
}

template <size_t N>
Matrix::Static<Int, Triangle<N>::nfp, Triangle<N>::np> Triangle<N>::RMatrix(
        const std::size_t s) const {
    // Creates node indices vector with node indices on face 1.
    Matrix::Static<Int,nfp,1> nodeVec;
    std::size_t last = 0;
    for (std::size_t i = 0; i < n + 1; i++) {
        last += i;
        nodeVec(i,0) = last;
    }
    // Creates extraction matrix R for face 1.
    Matrix::Static<Int,nfp,np> Raux;
    for (std::size_t i = 0; i < nfp; i++) {
        Raux(i,nodeVec(i,0)) = 1;
    }
    // Returns final form of R.
    Matrix::Static<Int,np,np> P;
    P = PMatrix(N,s);
    Matrix::Static<Int,nfp,np> res = Raux * P * P;
    return res;
}

//template <Int SIMPTRI_N>
//Matrix::Static<Real, Triangle<N>::np, Triangle<N>::np>
//Triangle<SIMPTRI_N>::getMassMatrix() const {
//    vector<Matrix::Static<Real,np,np> > cwaa;
//    cwaa = cubatureLagrangeProducts();
//    Matrix::Static<Real,np,np> res;
//    for (std::size_t c = 0; c < cwaa.size(); c++) {
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
//    vector<Evaluation> ca(np, Evaluation(triRule.np));
//    for (std::size_t i = 0; i < np; i++)
//        ca[i] = triRule.eval(lagr[i]);
//    return ca;
//}
// 
//template <Int SIMPTRI_N> inline
//vector<Matrix::Static<Real, Triangle<N>::np, Triangle<N>::np> >
//Triangle<SIMPTRI_N>::cubatureLagrangeProducts() const {
//    // PURPOSE:
//    // - Gets cubatured alpha and derived alpha for simplex element
//    std::size_t i, j, c;
//    TriangleRule triRule(3);
//    Matrix::Static<Real, np, np> zero;
//    vector<Matrix::Static<Real, np, np> > res(triRule.np,  zero);
//    vector<Evaluation> ca(np, Evaluation(triRule.np));
//    ca = cubatureLagrange();
//    // Computes Cubature weighted alpha by alpha products.
//    for (c = 0; c < triRule.np; c++)
//        for (i = 0; i < np; i++)
//            for (j = 0; j < np; j++)
//                res[c](i,j) = triRule.w[c] * ca[i].val[c] * ca[j].val[c];
//    return res;
//}
// 
//template <Int SIMPTRI_N> inline
//void
//Triangle<SIMPTRI_N>::computeWeights() {
//    for (std::size_t i = 0; i < np; i++)
//        weight[i] = lagr[i].integrateInSimplexSpace();
//}

template <size_t N>
void Triangle<N>::buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                                           const std::size_t order,
                                           const std::size_t nNodes) const {
    // Computes first coordinate indices vector.
    Matrix::Dynamic<Int> ini(nNodes,nsc);
    for (std::size_t i = 0; i <= order; i++)
        for (std::size_t j = numberOfNodes(std::size_t(i-1)); j < nNodes; j++)
            ini(j,0) = order - i;
    // Computes ordered nodal simplex coordinates.
    Matrix::Dynamic<Int> ord(nNodes,nsc);
    for (std::size_t i = 0; i < nsc; i++) {
        ord = PMatrix(order,i) * ini;
        // Stores in nId matrix.
        for (std::size_t j = 0; j < nNodes; j++) {
            res[j](i) = ord(j,0);
        }
    }
}

template <size_t N>
void Triangle<N>::printInfo() const {
    std::cout << " --- Simplex::Triangle Info --- "   << std::endl;
    Simplex::printInfo();
    std::cout << " Rotation matrices:             "        << std::endl;
    for (std::size_t i = 0; i < nsc; i++) {
        std::cout << "Rotation around simplex coordinate #" << i << std::endl;
        PMatrix(N,i).printInfo();
    }
    std::cout << " Extraction matrices:           "        << std::endl;
    for (std::size_t i = 0; i < nsc; i++) {
        std::cout << "Extraction matrices for face #" << i << std::endl;
        RMatrix(i).printInfo();
    }
    std::cout << " --- End of simplex information --- "   << std::endl;
}

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */
