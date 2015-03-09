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

Tri3::Tri3(const CoordinateGroup<>& coordGr,
           const ElementId id,
           const CoordinateId vId[3],
           const LayerId layerId,
           const uint matId)
:   Tri(id, layerId, matId) {
    
	for (uint i = 0; i < geo.np; i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Tri3::Tri3(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->is<CoordD3>()) {
            cerr << "ERROR @ Tri3::Tri3(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v_[i] = coord->castTo<CoordD3>();
	}
	check();
}

Tri3::Tri3(const ElementId id,
           const CoordD3* v[3],
           const LayerId layerId,
           const uint matId)
:   Tri(id, layerId, matId) {
    for (uint i = 0; i < geo.np; i++) {
        v_[i] = v[i];
    }
    check();
}

Tri3::Tri3(const Tri3& rhs)
:   Tri(rhs) {
    
    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tri3::~Tri3() {

}

ClassBase* Tri3::clone() const {
    return new Tri3(*this);
}

const CoordD3* Tri3::getSideV(const uint f, const uint i) const {
    assert(f < numberOfFaces());
    assert(i < numberOfSideCoordinates());
    return v_[geo.sideNode(f, i)];
}

const CoordD3* Tri3::getVertex(const uint i) const {
	return v_[geo.vertex(i)];
}

const CoordD3* Tri3::getSideVertex(const uint f, const uint i) const {
	assert(f < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v_[geo.sideVertex(f, i)];
}

double Tri3::getArea() const {
	CVecD3 v1, v2;
	v1 = getVertex(1)->pos() - getVertex(0)->pos();
	v2 = getVertex(2)->pos() - getVertex(0)->pos();
	return ((double) 0.5 * (v1 ^ v2).norm());
}

void Tri3::getCubatureDifferentials(double csdf[SimplexTri<1>::ncp]) const {
	CVecD3 csTanVec[geo.ncp];
	getCubatureTangentsVecProds(csTanVec);
	for (uint c = 0; c < geo.ncp; c++) {
		csdf[c] = csTanVec[c].norm();
	}
}

void Tri3::getCubatureNormals(CVecD3 csdn[SimplexTri<1>::ncp]) const {
	CVecD3 cTanVec[geo.ncp];
	getCubatureTangentsVecProds(cTanVec);
	for (uint c = 0; c < geo.ncp; c++) {
		csdn[c] = cTanVec[c].normalize();
	}
}

void Tri3::getCubatureNodes(CVecD3 cNode[SimplexTri<1>::ncp]) const {
	// Evaluates Lagrange's functions in positions specified by the
	for (uint i = 0; i < geo.np; i++) {
		for (uint c = 0; c < geo.ncp; c++) {
			cNode[c] += *getV(i) * geo.ca[i][c];
		}
	}
}

void Tri3::setV(const uint i, const CoordD3* vNew) {
    v_[i] = vNew;
}

void Tri3::check() const {

}

void Tri3::printInfo() const {
	cout << "--- Tri3 info ---" << endl;
	Element::printInfo();
	cout << "Coordinates:" << endl;
	for (uint i = 0; i < numberOfCoordinates(); i++) {
	    v_[i]->printInfo();
		cout << endl;
	}
}

Tri3 Tri3::linearize() const {
	return *this;
}

void Tri3::getCubatureTangentsVecProds(
    CVecD3 cTanVecProd[SimplexTri<2>::ncp]) const {

    StaMatrix<double,3,3> csJ;
    uint j, i, c;
    // Gets cubature points for base Lagrange polynomials.
    CVecD3 auxPos, ct1, ct2;
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
