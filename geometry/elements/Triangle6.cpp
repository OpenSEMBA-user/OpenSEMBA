
#ifndef TRI6_H_
#include <geometry/elements/Triangle6.h>
#endif

Triangle6::Triangle6() {

}

Triangle6::Triangle6(const GroupCoordinates<CoordR3>& coordGr,
           const ElementId id,
           const CoordinateId vId[6],
           const LayerId layerId,
           const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

	for (UInt i = 0; i < geo.np; i++) {
        v_[i] = coordGr.getId(vId[i]);
	}
}

Triangle6::Triangle6(const ElementId id,
           const CoordR3* v[6],
           const LayerId layerId,
           const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    for (UInt i = 0; i < geo.np; i++) {
        v_[i] = v[i];
    }
}

Triangle6::Triangle6(const Triangle6& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {

    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Triangle6::~Triangle6() {

}

bool Triangle6::isCurved() const {
    CVecR3 cn[geo.ncp];
    getCubatureNormals(cn);
    CVecR3 v1,v2,pN;
    v1 = getVertex(1)->pos() - getVertex(0)->pos();
    v2 = getVertex(2)->pos() - getVertex(0)->pos();
    pN = (v1 ^ v2).normalize();
    Real curvature = (Real) 0.0;
    for (UInt c = 0; c < geo.ncp; c++) {
        curvature += geo.cw[c] * (1.0 - abs(cn[c].dot(pN)));
    }
    if (MathUtils::notEqual(curvature, 0.0, 1.0)) {
        return true;
    }
    return false;
// The strategy to determine curvature is to check if side base nodes are
// Aligned using vector product.
//  CartesianVector<Real,3> vec2, vec1;
//  bool aligned;
//  Real alignement;
//  for (UInt s = 0; s < tri.faces; s++) {
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

const CoordR3* Triangle6::getSideV(const UInt face, const UInt i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideCoordinates());
	return v_[geo.sideNode(face,i)];
}

const CoordR3* Triangle6::getVertex(const UInt i) const {
    return v_[geo.vertex(i)];
}

const CoordR3* Triangle6::getSideVertex(const UInt face, const UInt i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v_[geo.sideVertex(face,i)];
}

Real Triangle6::getArea() const {
	Real csdf[geo.ncp];
	getCubatureDifferentials(csdf);
	Real res = 0.0;
	for (UInt c = 0; c < geo.ncp; c++) {
		res += geo.cw[c] * csdf[c];
	}
	res *= 0.5;
	return res;
}

void Triangle6::getCubatureDifferentials(Real csdf[SimplexTri<2>::ncp]) const {
    CVecR3 csTanVec[geo.ncp];
    getCubatureTangentsVecProds(csTanVec);
    for (UInt c = 0; c < geo.ncp; c++) {
        csdf[c] = csTanVec[c].norm();
    }
}

void Triangle6::getCubatureNormals(CVecR3 csdn[SimplexTri<2>::ncp]) const {
    CVecR3 cTanVec[geo.ncp];
    getCubatureTangentsVecProds(cTanVec);
    for (UInt c = 0; c < geo.ncp; c++) {
        csdn[c] = cTanVec[c].normalize();
    }
}

void Triangle6::getCubatureNodes(CVecR3 cNode[SimplexTri<2>::ncp]) const {
    // Evaluates Lagrange's functions in positions specified by the
    for (UInt i = 0; i < geo.np; i++) {
        for (UInt c = 0; c < geo.ncp; c++) {
            cNode[c] += *getV(i) * geo.ca[i][c];
        }
    }
}

void Triangle6::setV(const UInt i, const CoordR3* vNew) {
    v_[i] = vNew;
}

Triangle3* Triangle6::linearize() const {
	const CoordR3* vertex[3];
	for (UInt i = 0; i < 3; i++) {
		vertex[i] = getVertex(i);
	}
	return new Triangle3(getId(), vertex, getLayerId(), getMatId());
}

void Triangle6::printInfo() const {
    cout << "--- Tri6 info ---" << endl;
    cout << "Id: " << getId() << "  Mat Id: " << getMatId() << endl;
    cout << "Coordinates:" << endl;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

void
Triangle6::getCubatureTangentsVecProds(
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
