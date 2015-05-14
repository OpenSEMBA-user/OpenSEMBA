#ifndef SIMPLEXTET_H_
	#include "SimplexTet.h"
#endif

// =============== SimplexTet =================================================
// =-=-=-=-=-=-=-= Constructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template <Int SIMPTET_N>
SimplexTet<SIMPTET_N>::SimplexTet() {
	for (UInt s = 0; s < nsc; s++) {
		P[s] = PMatrix(n,s);
	}
	buildNodeIndices(nId,n,np);
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
	// ----------- Cubature positions and weights -----------------------------
	buildCubaturePositionsAndWeights();
	// ----------- Cubature lagranges -----------------------------------------
	buildCubatureLagrange();
	buildLIFTMatrix();
}

template <Int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildCubaturePositionsAndWeights() {
	buildNodeIndices(cId,nc,ncp);
	CartesianVector<Real,nsc> aux;
	for (UInt i = 0; i < ncp; i++) {
		aux = cId[i];
		cPos[i] = aux / (Real) nc;
	}
	Polynomial<Real> cubLagr[ncp];
	cubatureLagrangePolynomials(cubLagr,nc,ncp,nsc);
	for (UInt i = 0; i < ncp; i++) {
		cw[i] = integrate(cubLagr[i], dimension, sizeFactor) / sizeFactor;
	}
}

template <Int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildNodeIndices(
 CartesianVector<Int,nsc> *res,
 const UInt order,
 const UInt nNodes) const {
	// Computes first coordinate indices vector.
	DynMatrix<Int> ini(nNodes,nsc);
	for (UInt i = 0; i <= order; i++) {
		for (UInt j = numberOfNodes(i-1); j < nNodes; j++) {
			ini(j,0) = order - i;
		}
	}
	// Computes ordered nodal simplex coordinates.
	DynMatrix<Int> ord(nNodes,nsc);
	for (UInt i = 0; i < nsc; i++) {
		ord = PMatrix(order,i) * ini;
		// Stores in nId matrix.
		for (UInt j = 0; j < nNodes; j++) {
			res[j](i) = ord(j,0);
		}
	}
}

 
template<Int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildCubatureLagrange() {
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
 
template <Int SIMPTET_N>
inline UInt
SimplexTet<SIMPTET_N>::nodeIndex(const UInt i, const UInt j) const {
	return nId[i](j);
}

template <Int SIMPTET_N>
inline UInt
SimplexTet<SIMPTET_N>::cubatureNodeIndex(const UInt i, const UInt j) const {
	return cId[i](j);
}

template <Int SIMPTET_N>
inline const Polynomial<Real>&
SimplexTet<SIMPTET_N>::getLagr(const UInt i) const {
	return lagr[i];
}

template <Int SIMPTET_N>
inline const Polynomial<Real>&
SimplexTet<SIMPTET_N>::getDLagr(const UInt i, const UInt f) const {
	return dLagr[i][f];
}

template <Int SIMPTET_N>
Real
SimplexTet<SIMPTET_N>::getCda(UInt i, UInt j, UInt k) const {
	return cda[i][j][k];
}
 
template <Int SIMPTET_N>
StaMatrix<Real, SIMPTET_NP, SIMPTET_NP>
SimplexTet<SIMPTET_N>::getMassMatrix() const {
    StaMatrix<Real,np,np> res;
	for (UInt c = 0; c < ncp; c++) {
		res += cwaa[c];
	}
	return res;
}

template <Int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildLIFTMatrix() {
	// Obtains mass matrix for the tetrahedron.
	StaMatrix<Real,np,np> invM;
	invM = getMassMatrix().invert();
	// Obtains side mass matrix using simplex triangle.
    StaMatrix<Real, nfp, nfp> sM;
	sM = tri.getMassMatrix();
	// Computes LIFT matrix.
	StaMatrix<Int, np, nfp> RtrInt;
	StaMatrix<Real, np, nfp> RtrDbl;
	for (UInt f = 0; f < faces; f++) {
		RtrInt = R[f].transpose();
		RtrDbl.assign(RtrInt);
		LIFT[f] = RtrDbl * sM;
		LIFT[f] = invM * LIFT[f];
	}
}
 
template <Int SIMPTET_N>
inline UInt
SimplexTet<SIMPTET_N>::vertex(const UInt vertexNum) const {
	return sideNode(vertexNum,0);
}
 
template <Int SIMPTET_N>
inline UInt
SimplexTet<SIMPTET_N>::sideVertex(
 const UInt face, const UInt vertexNum) const {
	return sideNode(face, tri.vertex(vertexNum));
}
 
template <Int SIMPTET_N>
inline UInt
SimplexTet<SIMPTET_N>::sideNode(
 const UInt face, const UInt num) const {
	return sNId(face, num);
}
 
template <Int SIMPTET_N>
Real
SimplexTet<SIMPTET_N>::integrateScalarsOnFace(
 const Real x[SIMPTET_NP],
 const UInt f,
 const Real area) const {
	// Takes an array of np scalars, a face number f and an area.
	// Returns a vector that is the result of integrating over the face of
	// the simplex and multiply the result by the area.
	Real faceX[nfp];
    for (UInt i = 0; i < nfp; i++)
    	faceX[i] = x[sideNode(f,i)];
    // Computes result.
    Real res;
    for (UInt i = 0; i < nfp; i++) {
    	for (UInt c = 0; c < ncp; c++) {
    		res += tri.cw[c] * faceX[i];
    	}
    }
    res *= area;
    throw ErrorNotImplemented("SimplexTet::integrateScalarsOnFace");
	return res;
}
 
template <Int SIMPTET_N>
Real
SimplexTet<SIMPTET_N>::integrateScalars(
  const Real x[SIMPTET_NP],
  const Real volume) const {
	Real res = 0.0;
    for (UInt i = 0; i < np; i++) {
    	for (UInt c = 0; c < ncp; c++) {
    		res += cw[c] * ca[i][c] * x[i];
    	}
    }
    return (res*volume);
}

template <Int SIMPTET_N>
CartesianVector<Real,4>
SimplexTet<SIMPTET_N>::coordinate(const UInt i) const {
	CartesianVector<Real,4> res;
	res = nId[i];
	res /= (Real) n;
	return res;
}

template <Int SIMPTET_N>
CartesianVector<Real,4>
SimplexTet<SIMPTET_N>::sideCoordinate(
 const UInt f, const UInt i) const {
	return coordinate(sideNode(f,i));
}
 
template <Int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildSideNodeIndices() {
	StaMatrix<Int,np,1> nList;
	StaMatrix<Int,nfp,1> aux;
	// Initializes node list.
	for (UInt i = 0; i < np; i++) {
		nList(i,0) = i;
	}
	// Creates aux matrix to store the computed sNId.
	for (UInt f = 0; f < faces; f++) {
		aux = R[f] * nList;
		for (UInt i = 0; i < nfp; i++)
			sNId(f,i) = aux(i,0);
	}
}
 
template<Int SIMPTET_N>
Int
SimplexTet<SIMPTET_N>::numberOfNodes(Int order) const {
	Int res = 1;
	for (Int i = 1; i < Int(nsc); i++)
		res *= (order + i);
	res /= factorial(nsc - 1);
	return res;
}
 
template <Int SIMPTET_N>
DynMatrix<Int>
SimplexTet<SIMPTET_N>::PMatrix(const UInt n, const UInt s) const {
	// Purpose: Creates indices rotation matrix for a simplex tetrahedral
	//          element in 3-D.
	UInt np = numberOfNodes(n);
	DynMatrix<Int> res(np,np);
	if (s == 0) {
		res.eye();
	} else if (s >= 1 && s <= 3) {
		// P2Dall will store the rotation matrix for all the indices slices.
		DynMatrix<Int> P2Dall(np,np);
		UInt nodesSet = 0;
		for (UInt i = 0; i <= n; i++) {
			UInt sliceNP = (i + 1) * (i + 2) / 2;
			// Assign P2D to P2Dall matrix.
			DynMatrix<Int> auxP = tri.PMatrix(i,2);
			for (UInt j = 0; j < sliceNP; j++) {
				for (UInt k = 0; k < sliceNP; k++) {
					P2Dall(j+nodesSet,k+nodesSet) = auxP.val(j,k);
				}
			}
			nodesSet += sliceNP;
		}
		// Allocates and initializes ordered and final indices vectors.
		DynMatrix<Int> orVec(np,1), fiVec(np,1), temp(np,1);
		for (UInt i = 0; i < np; i++) {
			orVec(i,0) = i;
		}
		// Computes final indices positions.
		fiVec = P2Dall * orVec;
		// Assigns ones to the Q1 matrix.
		DynMatrix<Int> Q[faces];
		DynMatrix<Int> QInit(np,np);
		for (UInt i = 0; i < faces; i++) {
			Q[i] = QInit;
		}
		for (UInt i = 0; i < np; i++) {
			Q[0](orVec(i,0),fiVec(i,0)) = 1;
		}
		// Defines Q[1], same as Q[0] but around vertex 1.
		// Assigns nodes to the original (non-rotated) enumeration vector.
		orVec.zeros();
		UInt lastSet = n * (n+1) * (n+2) / 6 + 1;
		UInt lastNodeSet = 0;
		for (UInt i = 0; i <= n; i++)
			for (UInt j = 1; j <= i+1; j++) {
				orVec(lastNodeSet+j-1,0) = lastSet;
				lastSet += 1;
				lastNodeSet += j;
			}
		lastSet = n * (n+1) * (n+2) / 6 + 1;
		for (UInt j = 1; j <= n; j++) {
			temp = orVec;
			for(Int i = Int(np); i >= 2; i--) {
				if (temp(i-1,0) != 0)
					if (temp(i-2,0) == 0) {
						lastSet--;
						orVec(i-2,0) = lastSet;
					}
			}
		}
		// Creates final (rotated) enumeration vector.
		fiVec = P2Dall * orVec;
		// Assigns values to Q[1].
		for (UInt i = 0; i < np; i++) {
			Q[1](orVec(i,0)-1,fiVec(i,0)-1) = 1;
		}
		DynMatrix<Int> Q0Sq, Q1Sq;
		Q0Sq = Q[0] * Q[0];
		Q1Sq = Q[1] * Q[1];
		switch (s) {
		case 1:
			Q[2] = Q1Sq * Q0Sq; // Q[2] = Q[1] * Q[1] * Q[0] * Q[0];
			res = Q[2] * Q1Sq;  // res = Q[2] * Q[1] * Q[1];
			break;
		case 2:
			Q[2] = Q1Sq * Q0Sq; // Q[2] = Q[1] * Q[1] * Q[0] * Q[0];
			Q[3] = Q0Sq * Q1Sq; // Q[3] = Q[0] * Q[0] * Q[1] * Q[1];
			res = Q[3] * Q[2] * Q[2];
			break;
		case 3:
			Q[3] = Q0Sq * Q1Sq;// Q[3] = Q[0] * Q[0] * Q[1] * Q[1];
			res = Q[1] * Q[3] * Q[3];
			break;
		}
	}
	return res;
}
 
template <Int SIMPTET_N>
StaMatrix<Int,SIMPTET_NFP,SIMPTET_NP>
SimplexTet<SIMPTET_N>::RMatrix(const UInt s) const {
	// Creates node indices vector with node indices on face 1.
	UInt last = 0;
	StaMatrix<Int,SIMPTET_NFP,1> nodeVec;
	for (UInt i = 1; i <= n + 1; i++) {
		UInt nsp = (i-1) * (i) * (i+1) / 6;
		for (UInt j = 0; j<i; j++) {
			nodeVec(last + j,0) = nsp + (j)*(j+1)/2 + 1;
		}
		last += i;
	}
	// Creates extraction matrix R for face 1.
	StaMatrix<Int,SIMPTET_NFP,SIMPTET_NP> Raux;
	for (UInt i = 0; i < nfp; i++) {
		Raux(i,nodeVec(i,0)-1) = 1;
	}
	// Returns final form of R.
	StaMatrix<Int,SIMPTET_NFP,SIMPTET_NP> res = Raux * P[s];
	return res;
}

template <Int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::printInfo() const {
	cout << " --- SimplexTet Information --- " << endl;
	cout << " Number of coordinates:         " << faces << endl;
	cout << " Order:                         " << n << endl;
	cout << " Number of nodes:               " << np << endl;
	cout << " Number of face nodes:          " << nfp << endl;
	cout << " Rotation matrices:             " << endl;
	for (UInt i = 0; i < faces; i++) {
		cout << "Rotation around simplex coordinate #" << i << endl;
		P[i].printInfo();
	}
	cout << " Extraction matrices:           " << endl;
	for (UInt i = 0; i < faces; i++) {
		cout << "Extraction matrices for face #" << i << endl;
		R[i].printInfo();
	}
	cout << " List of node indices: " << endl;
	for (UInt i = 0; i < np; i++) {
		nId[i].printInfo();
		cout << endl;
	}
	cout << " List of node coordinates: " << endl;
	for (UInt i = 0; i < np; i++) {
		coordinate(i).printInfo();
		cout << endl;
	}
	cout << " List of side nodes indices:    " << endl;
	sNId.printInfo();
	cout << " List of side nodes coordinates: " << endl;
	for (UInt f = 0; f < faces; f++) {
		cout << "Face #" << f << endl;
		for (UInt i = 0; i < nfp; i++) {
			sideCoordinate(f,i).printInfo();
			cout << endl;
		}
	}
	cout << " Lagrange polynomials:          " << endl;
	for (UInt i = 0; i < np; i++) {
		cout << "Lagrange polynomial of node #" << i << endl;
		lagr[i].printInfo();
	}
	cout << " Lagrange polynomials derivatives: " << endl;
	for (UInt i = 0; i < np; i++)
		for (UInt j = 0; j < faces; j++) {
			cout << "Pol. " << i << " derived w.r.t. var." << j << endl;
			dLagr[i][j].printInfo();
		}
	cout << " LIFT Matrices" << endl;
	for (UInt i = 0; i < faces; i++)
		LIFT[i].printInfo();
	cout << " Order of cubature: " << nc << endl;
	cout << " Number of cub. points: " << ncp << endl;
	cout << " Cubature positions and weights: " << endl;
	for (UInt i = 0; i < ncp; i++) {
		cout << "#" << i << " ";
		cPos[i].printInfo();
		cout << " " << cw[i] << endl;
	}
	Real sum = 0.0;
	for (UInt i = 0; i < ncp; i++) {
		sum += cw[i];
	}
	cout << " The sum of cubature weights is: " << sum << endl;
	cout << " --- End of simplex information --- "   << endl;
}
