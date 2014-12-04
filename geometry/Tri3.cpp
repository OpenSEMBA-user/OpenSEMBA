/*
 * Tri3.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TRI3_H_
#include "Tri3.h"
#endif

Tri3::Tri3() {

}

Tri3::~Tri3() {

}

Tri3::Tri3(
 const CoordinateGroup& coordGr,
 const uint vId[3],
 const CVecD3& normal_,
 const uint id_, const uint matId_, const uint layerId_) :
         Tri(normal_, id_, matId_, layerId_) {
	for (uint i = 0; i < geo.np; i++) {
		v[i] = coordGr.getPtrToId(vId[i]);
	}
	check();
}

Tri3::Tri3(
 const CoordD3* v_[3],
 const CVecD3 normal_,
 const uint id_, const uint matId_, const uint layerId_) :
        Tri(normal_, id_, matId_, layerId_) {
	for (uint i = 0; i < geo.np; i++) {
		v[i] = v_[i];
	}
	check();
}

Tri3&
Tri3::operator=(const Tri3& rhs) {
	if (this == &rhs) {
		return *this;
	}
	Tri::operator=(rhs);
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		v[i] = rhs.v[i];
	}
	return *this;
}

void
Tri3::setV(const uint i, const CoordD3* vNew) {
	v[i] = vNew;
}

const CoordD3*
Tri3::getVertex(const uint i) const {
	return v[geo.vertex(i)];
}

const CoordD3*
Tri3::getSideV(const uint face, const uint i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideCoordinates());
	return v[geo.sideNode(face, i)];
}

const CoordD3*
Tri3::getSideVertex(const uint face, const uint i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v[geo.sideVertex(face, i)];
}

double
Tri3::getArea() const {
	CVecD3 v1, v2;
	v1 = getVertex(1)->pos() - getVertex(0)->pos();
	v2 = getVertex(2)->pos() - getVertex(0)->pos();
	return ((double) 0.5 * (v1 ^ v2).norm());
}

void
Tri3::getCubatureDifferentials(
 double csdf[SimplexTri<1>::ncp]) const {
	CVecD3 csTanVec[geo.ncp];
	getCubatureTangentsVecProds(csTanVec);
	for (uint c = 0; c < geo.ncp; c++) {
		csdf[c] = csTanVec[c].norm();
	}
}

void
Tri3::getCubatureNormals(
 CVecD3 csdn[SimplexTri<1>::ncp]) const {
	CVecD3 cTanVec[geo.ncp];
	getCubatureTangentsVecProds(cTanVec);
	for (uint c = 0; c < geo.ncp; c++) {
		csdn[c] = cTanVec[c].normalize();
	}
}

void
Tri3::getCubatureNodes(
 CVecD3 cNode[SimplexTri<1>::ncp]) const {
	// Evaluates Lagrange's functions in positions specified by the
	for (uint i = 0; i < geo.np; i++) {
		for (uint c = 0; c < geo.ncp; c++) {
			cNode[c] += *getV(i) * geo.ca[i][c];
		}
	}
}

void
Tri3::getCubatureTangentsVecProds(
 CVecD3 cTanVecProd[SimplexTri<2>::ncp]) const {
	StaMatrix<double,3,3> csJ;
	uint j, i, c;
	// Gets cubature points for base Lagrange polynomials.
	CVecD3 auxPos, ct1, ct2;
	for (c = 0; c < geo.ncp; c++) {
		csJ.zeros();
		for (i = 0; i < geo.np; i++) {
			for (j = 0; j < geo.nsc; j++) {
				auxPos = *v[i] * geo.getDLagr(i,j).eval(geo.cPos[c]);
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

void
Tri3::check() const {

}

void
Tri3::printInfo() const {
	cout << "--- Tri3 info ---" << endl;
	cout << "Id: " << id << "  Mat Id: " << matId << endl;
	cout << "Coordinates:" << endl;
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		v[i]->printInfo();
		cout << endl;
	}
}

Tri3
Tri3::linearize() const {
	return *this;
}
