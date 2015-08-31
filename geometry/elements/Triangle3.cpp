/*
 * Tri3.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TRI3_H_
#include <geometry/elements/Triangle3.h>
#endif

Triangle3::Triangle3() {

}

Triangle3::Triangle3(const GroupCoordinates<CoordR3>& coordGr,
           const ElementId id,
           const CoordinateId vId[3],
           const LayerId layerId,
           const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    
	for (UInt i = 0; i < geo.np; i++) {
        v_[i] = coordGr.getId(vId[i]);
	}
	check();
}

Triangle3::Triangle3(const ElementId id,
           const CoordR3* v[3],
           const LayerId layerId,
           const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    for (UInt i = 0; i < geo.np; i++) {
        v_[i] = v[i];
    }
    check();
}

Triangle3::Triangle3(const Triangle3& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {
    
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Triangle3::~Triangle3() {

}

const CoordR3* Triangle3::getSideV(const UInt f, const UInt i) const {
    assert(f < numberOfFaces());
    assert(i < numberOfSideCoordinates());
    return v_[geo.sideNode(f, i)];
}

const CoordR3* Triangle3::getVertex(const UInt i) const {
	return v_[geo.vertex(i)];
}

const CoordR3* Triangle3::getSideVertex(const UInt f, const UInt i) const {
	assert(f < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v_[geo.sideVertex(f, i)];
}

Real Triangle3::getArea() const {
	CVecR3 v1, v2;
	v1 = getVertex(1)->pos() - getVertex(0)->pos();
	v2 = getVertex(2)->pos() - getVertex(0)->pos();
	return ((Real) 0.5 * (v1 ^ v2).norm());
}

void Triangle3::getCubatureDifferentials(Real csdf[SimplexTri<1>::ncp]) const {
	CVecR3 csTanVec[geo.ncp];
	getCubatureTangentsVecProds(csTanVec);
	for (UInt c = 0; c < geo.ncp; c++) {
		csdf[c] = csTanVec[c].norm();
	}
}

void Triangle3::getCubatureNormals(CVecR3 csdn[SimplexTri<1>::ncp]) const {
	CVecR3 cTanVec[geo.ncp];
	getCubatureTangentsVecProds(cTanVec);
	for (UInt c = 0; c < geo.ncp; c++) {
		csdn[c] = cTanVec[c].normalize();
	}
}

void Triangle3::getCubatureNodes(CVecR3 cNode[SimplexTri<1>::ncp]) const {
	// Evaluates Lagrange's functions in positions specified by the
	for (UInt i = 0; i < geo.np; i++) {
		for (UInt c = 0; c < geo.ncp; c++) {
			cNode[c] += *getV(i) * geo.ca[i][c];
		}
	}
}

void Triangle3::setV(const UInt i, const CoordR3* vNew) {
    v_[i] = vNew;
}

void Triangle3::check() const {

}

void Triangle3::printInfo() const {
	cout << "--- Tri3 info ---" << endl;
	ElementBase::printInfo();
	cout << "Coordinates:" << endl;
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
	    v_[i]->printInfo();
	}
}

Triangle3* Triangle3::linearize() const {
	return new Triangle3(*this);
}

void Triangle3::getCubatureTangentsVecProds(
    CVecR3 cTanVecProd[SimplexTri<2>::ncp]) const {

    StaMatrix<Real,3,3> csJ;
    UInt j, i, c;
    // Gets cubature points for base Lagrange polynomials.
    CVecR3 auxPos, ct1, ct2;
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
        // Surface differential is the norm of the vector product.
        cTanVecProd[c] = (ct1 ^ ct2);
    }
}
