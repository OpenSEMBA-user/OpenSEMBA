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

#include "Triangle6.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

const Math::Simplex::Triangle<2> Triangle6::geo;

Triangle6::Triangle6() {

}

Triangle6::Triangle6(const Id id,
                     const CoordR3* v[6],
                     const Layer* lay,
                     const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    for (std::size_t i = 0; i < geo.np; i++) {
        v_[i] = v[i];
    }
}

Triangle6::Triangle6(const Triangle6& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Triangle6::~Triangle6() {

}

bool Triangle6::isCurved() const {
    Math::CVecR3 cn[geo.ncp];
    getCubatureNormals(cn);
    Math::CVecR3 v1,v2,pN;
    v1 = getVertex(1)->pos() - getVertex(0)->pos();
    v2 = getVertex(2)->pos() - getVertex(0)->pos();
    pN = (v1 ^ v2).normalize();
    Math::Real curvature = (Math::Real) 0.0;
    for (std::size_t c = 0; c < geo.ncp; c++) {
        curvature += geo.cw[c] * (1.0 - abs(cn[c].dot(pN)));
    }
    if (Math::Util::notEqual(curvature, 0.0, 1.0)) {
        return true;
    }
    return false;
// The strategy to determine curvature is to check if side base nodes are
// Aligned using std::vector product.
//  CartesianVector<Math::Real,3> vec2, vec1;
//  bool aligned;
//  Math::Real alignement;
//  for (std::size_t s = 0; s < tri.faces; s++) {
//      vec1 = *v[tri.sideNode(s,1)] - *v[tri.sideNode(s,0)];
//      vec2 = *v[tri.sideNode(s,2)] - *v[tri.sideNode(s,0)];
//      vec1.normalize();
//      vec2.normalize();
//      alignement = 1 - vec1.scalarProd(vec2);
//      aligned = (alignement <= curvatureTolerance);
//      if(!aligned) {
//          return true;
//      }
//  }
//  return false;
//  return true;
//  #warning "Every tri6 surface is always treated as curved."
}

const CoordR3* Triangle6::getSideV(const std::size_t face,
                                   const std::size_t i) const {
    assert(face < numberOfFaces());
    assert(i < numberOfSideCoordinates());
    return v_[geo.sideNode(face,i)];
}

const CoordR3* Triangle6::getVertex(const std::size_t i) const {
    return v_[geo.vertex(i)];
}

const CoordR3* Triangle6::getSideVertex(const std::size_t face,
                                        const std::size_t i) const {
    assert(face < numberOfFaces());
    assert(i < numberOfSideVertices());
    return v_[geo.sideVertex(face,i)];
}

Math::Real Triangle6::getArea() const {
    Math::Real csdf[geo.ncp];
    getCubatureDifferentials(csdf);
    Math::Real res = 0.0;
    for (std::size_t c = 0; c < geo.ncp; c++) {
        res += geo.cw[c] * csdf[c];
    }
    res *= 0.5;
    return res;
}

void Triangle6::getCubatureDifferentials(
        Math::Real csdf[Math::Simplex::Triangle<2>::ncp]) const {
    Math::CVecR3 csTanVec[geo.ncp];
    getCubatureTangentsVecProds(csTanVec);
    for (std::size_t c = 0; c < geo.ncp; c++) {
        csdf[c] = csTanVec[c].norm();
    }
}

void Triangle6::getCubatureNormals(
        Math::CVecR3 csdn[Math::Simplex::Triangle<2>::ncp]) const {
    Math::CVecR3 cTanVec[geo.ncp];
    getCubatureTangentsVecProds(cTanVec);
    for (std::size_t c = 0; c < geo.ncp; c++) {
        csdn[c] = cTanVec[c].normalize();
    }
}

void Triangle6::getCubatureNodes(
        Math::CVecR3 cNode[Math::Simplex::Triangle<2>::ncp]) const {
    // Evaluates Lagrange's functions in positions specified by the
    for (std::size_t i = 0; i < geo.np; i++) {
        for (std::size_t c = 0; c < geo.ncp; c++) {
            cNode[c] += *getV(i) * geo.ca[i][c];
        }
    }
}

void Triangle6::setV(const std::size_t i, const CoordR3* vNew) {
    v_[i] = vNew;
}

Triangle3* Triangle6::linearize() const {
    const CoordR3* vertex[3];
    for (std::size_t i = 0; i < 3; i++) {
        vertex[i] = getVertex(i);
    }
    return new Triangle3(getId(), vertex, getLayer(), getModel());
}

void Triangle6::printInfo() const {
    std::cout << "--- Tri6 info ---" << std::endl;
    std::cout << "Id: " << getId() << "  Mat Id: " << getMatId() << std::endl;
    std::cout << "Coordinates:" << std::endl;
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

void
Triangle6::getCubatureTangentsVecProds(
    Math::CVecR3 cTanVecProd[Math::Simplex::Triangle<2>::ncp]) const {

    Math::Matrix::Static<Math::Real,3,3> csJ;
    std::size_t j, i, c;
    // Gets cubature points for base Lagrange polynomials.
    Math::CVecR3 auxPos, ct1, ct2;
    for (c = 0; c < geo.ncp; c++) {
        csJ.zeros();
        for (i = 0; i < geo.np; i++) {
            for (j = 0; j < geo.nsc; j++) {
                auxPos = *v_[i] * geo.getDLagr(i,j).eval(geo.cPos[c]);
                csJ(0,j) += auxPos(0);
                csJ(1,j) += auxPos(1);
                csJ(2,j) += auxPos(2);
            }
        }
        // Tangential vectors on cubature points.
        // Computes tangent vectors on cubature points.
        for (j = 0; j < geo.nsc; j++) {
            ct1(j) = csJ(j,0) - csJ(j,2);
            ct2(j) = csJ(j,1) - csJ(j,2);
        }
        // Surface differential is the norm of the std::vector product.
        cTanVecProd[c] = (ct1 ^ ct2);
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
