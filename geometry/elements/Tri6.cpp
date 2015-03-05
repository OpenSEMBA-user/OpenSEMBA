#ifndef TRI6_H_
#include "Tri6.h"
#endif

Tri6::Tri6() {

}

Tri6::Tri6(const CoordinateGroup<>& coordGr,
           const ElementId id,
           const CoordinateId vId[6],
           const uint layerId,
           const uint matId)
:   Tri(id, layerId, matId) {
	for (uint i = 0; i < geo.np; i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Tri6::Tri6(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->isOf<CoordD3>()) {
            cerr << "ERROR @ Tri6::Tri6(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v_[i] = coord->castTo<CoordD3>();
	}
}

Tri6::Tri6(const ElementId id,
           const CoordD3* v[6],
           const uint layerId,
           const uint matId)
:   Tri(id, layerId, matId) {
    for (uint i = 0; i < geo.np; i++) {
        v_[i] = v[i];
    }
}

Tri6::Tri6(const Tri6& rhs)
:   Tri(rhs) {

    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tri6::Tri6(const ElementId id, const Tri6& rhs)
:   Tri(id, rhs) {

    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tri6::~Tri6() {

}

ClassBase* Tri6::clone() const {
    return new Tri6(*this);
}

ClassBase* Tri6::clone(const ElementId id) const {
    return new Tri6(id, *this);
}

bool Tri6::isCurved() const {
    CVecD3 cn[geo.ncp];
    getCubatureNormals(cn);
    CVecD3 v1,v2,pN;
    v1 = getVertex(1)->pos() - getVertex(0)->pos();
    v2 = getVertex(2)->pos() - getVertex(0)->pos();
    pN = (v1 ^ v2).normalize();
    double curvature = (double) 0.0;
    for (uint c = 0; c < geo.ncp; c++) {
        curvature += geo.cw[c] * (1.0 - abs(cn[c].dot(pN)));
    }
    if (curvature >= curvatureTolerance) {
        return true;
    }
    return false;
// The strategy to determine curvature is to check if side base nodes are
// Aligned using vector product.
//  CartesianVector<double,3> vec2, vec1;
//  bool aligned;
//  double alignement;
//  for (uint s = 0; s < tri.faces; s++) {
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

const CoordD3* Tri6::getSideV(const uint face, const uint i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideCoordinates());
	return v_[geo.sideNode(face,i)];
}

const CoordD3* Tri6::getVertex(const uint i) const {
    return v_[geo.vertex(i)];
}

const CoordD3* Tri6::getSideVertex(const uint face, const uint i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v_[geo.sideVertex(face,i)];
}

double Tri6::getArea() const {
	double csdf[geo.ncp];
	getCubatureDifferentials(csdf);
	double res = 0.0;
	for (uint c = 0; c < geo.ncp; c++) {
		res += geo.cw[c] * csdf[c];
	}
	res *= 0.5;
	return res;
}

void Tri6::getCubatureDifferentials(double csdf[SimplexTri<2>::ncp]) const {
    CVecD3 csTanVec[geo.ncp];
    getCubatureTangentsVecProds(csTanVec);
    for (uint c = 0; c < geo.ncp; c++) {
        csdf[c] = csTanVec[c].norm();
    }
}

void Tri6::getCubatureNormals(CVecD3 csdn[SimplexTri<2>::ncp]) const {
    CVecD3 cTanVec[geo.ncp];
    getCubatureTangentsVecProds(cTanVec);
    for (uint c = 0; c < geo.ncp; c++) {
        csdn[c] = cTanVec[c].normalize();
    }
}

void Tri6::getCubatureNodes(CVecD3 cNode[SimplexTri<2>::ncp]) const {
    // Evaluates Lagrange's functions in positions specified by the
    for (uint i = 0; i < geo.np; i++) {
        for (uint c = 0; c < geo.ncp; c++) {
            cNode[c] += *getV(i) * geo.ca[i][c];
        }
    }
}

void Tri6::setV(const uint i, const CoordD3* vNew) {
    v_[i] = vNew;
}

Tri3 Tri6::linearize() const {
	const CoordD3* vertex[3];
	for (uint i = 0; i < 3; i++) {
		vertex[i] = getVertex(i);
	}
	Tri3 res(getId(), vertex, getLayerId(), getMatId());
	return res;
}

void Tri6::printInfo() const {
    cout << "--- Tri6 info ---" << endl;
    cout << "Id: " << getId() << "  Mat Id: " << getMatId() << endl;
    cout << "Coordinates:" << endl;
    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
        cout << endl;
    }
}

void
Tri6::getCubatureTangentsVecProds(
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
