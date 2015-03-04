#ifndef TRI6_H_
#include "Tri6.h"
#endif

Tri6::Tri6(
 const CoordinateGroup<>& coordGr,
 const unsigned int id_,
 const unsigned int matId_,
 const unsigned int vId[6]) : Tri(id_, matId_) {
	for (unsigned int i = 0; i < geo.np; i++) {
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
        v[i] = coord->castTo<CoordD3>();
	}
}

void
Tri6::setV(const unsigned int i, const Coordinate<double,3>* vNew) {
	v[i] = vNew;
}

Tri6::Tri6(const Coordinate<double,3>* auxV[6]) : Tri(0) {
	for (unsigned int i = 0; i < geo.np; i++) {
		v[i] = auxV[i];
	}
}

const Coordinate<double,3>*
Tri6::getVertex(const unsigned int i) const {
	return v[geo.vertex(i)];
}

const Coordinate<double,3>*
Tri6::getSideV(const unsigned int face, const unsigned int i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideCoordinates());
	return v[geo.sideNode(face,i)];
}

const Coordinate<double, 3>*
Tri6::getSideVertex(const unsigned int face, const unsigned int i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v[geo.sideVertex(face,i)];
}

void
Tri6::printInfo() const {
	cout << "--- Tri6 info ---" << endl;
	cout << "Id: " << id << "  Mat Id: " << matId << endl;
	cout << "Coordinates:" << endl;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		v[i]->printInfo();
		cout << endl;
	}
}

double
Tri6::getArea() const {
	double csdf[geo.ncp];
	getCubatureDifferentials(csdf);
	double res = 0.0;
	for (unsigned int c = 0; c < geo.ncp; c++) {
		res += geo.cw[c] * csdf[c];
	}
	res *= 0.5;
	return res;
}

Tri3
Tri6::linearize() const {
	const Coordinate<double,3>* vertex[3];
	for (unsigned int i = 0; i < 3; i++) {
		vertex[i] = getVertex(i);
	}
	Tri3 res(vertex, id, matId, layerId);
	return res;
}

void
Tri6::getCubatureTangentsVecProds(
 CartesianVector<double,3> cTanVecProd[SimplexTri<2>::ncp]) const {
	StaMatrix<double,3,3> csJ;
	unsigned int j, i, c;
	// Gets cubature points for base Lagrange polynomials.
	CartesianVector<double,3> auxPos, ct1, ct2;
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
Tri6::getCubatureDifferentials(
 double csdf[SimplexTri<2>::ncp]) const {
	CartesianVector<double,3> csTanVec[geo.ncp];
	getCubatureTangentsVecProds(csTanVec);
	for (unsigned int c = 0; c < geo.ncp; c++) {
		csdf[c] = csTanVec[c].norm();
	}
}

void
Tri6::getCubatureNormals(
 CartesianVector<double,3> csdn[SimplexTri<2>::ncp]) const {
	CartesianVector<double,3> cTanVec[geo.ncp];
	getCubatureTangentsVecProds(cTanVec);
	for (unsigned int c = 0; c < geo.ncp; c++) {
		csdn[c] = cTanVec[c].normalize();
	}
}

void
Tri6::getCubatureNodes(
 CartesianVector<double,3> cNode[SimplexTri<2>::ncp]) const {
	// Evaluates Lagrange's functions in positions specified by the
	for (unsigned int i = 0; i < geo.np; i++) {
		for (unsigned int c = 0; c < geo.ncp; c++) {
			cNode[c] += *getV(i) * geo.ca[i][c];
		}
	}
}

bool
Tri6::isCurved() const {
	CartesianVector<double,3> cn[geo.ncp];
	getCubatureNormals(cn);
	CartesianVector<double,3> v1,v2,pN;
	v1 = getVertex(1)->pos() - getVertex(0)->pos();
	v2 = getVertex(2)->pos() - getVertex(0)->pos();
	pN = (v1 ^ v2).normalize();
	double curvature = (double) 0.0;
	for (unsigned int c = 0; c < geo.ncp; c++) {
		curvature += geo.cw[c] * (1.0 - abs(cn[c].dot(pN)));
	}
	if (curvature >= curvatureTolerance) {
		return true;
	}
	return false;
// The strategy to determine curvature is to check if side base nodes are
// Aligned using vector product.
//	CartesianVector<double,3> vec2, vec1;
//	bool aligned;
//	double alignement;
//	for (unsigned int s = 0; s < tri.faces; s++) {
//		vec1 = *v[tri.sideNode(s,1)] - *v[tri.sideNode(s,0)];
//		vec2 = *v[tri.sideNode(s,2)] - *v[tri.sideNode(s,0)];
//		vec1.normalize();
//		vec2.normalize();
//		alignement = 1 - vec1.scalarProd(vec2);
//		aligned = (alignement <= curvatureTolerance);
//		if(!aligned) {
//			return true;
//		}
//	}
//	return false;
//	return true;
//	#warning "Every tri6 surface is always treated as curved."
}

