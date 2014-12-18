#ifndef SIMPLEXTET_H_
	#include "SimplexTet.h"
#endif

// =============== SimplexTet =================================================
// =-=-=-=-=-=-=-= Constructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template <int SIMPTET_N>
SimplexTet<SIMPTET_N>::SimplexTet() {
	for (unsigned int s = 0; s < nsc; s++) {
		P[s] = PMatrix(n,s);
	}
	buildNodeIndices(nId,n,np);
	for (unsigned int s = 0; s < nsc; s++) {
		R[s] = RMatrix(s);
	}
	buildSideNodeIndices();
	lagrangePolynomials(lagr,n,np,nsc);
	for (unsigned int i = 0; i < np; i++) {
		for (unsigned int s = 0; s < nsc; s++) {
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

template <int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildCubaturePositionsAndWeights() {
	buildNodeIndices(cId,nc,ncp);
	CartesianVector<double,nsc> aux;
	for (unsigned int i = 0; i < ncp; i++) {
		aux = cId[i];
		cPos[i] = aux / (double) nc;
	}
	Polynomial<double> cubLagr[ncp];
	cubatureLagrangePolynomials(cubLagr,nc,ncp,nsc);
	for (unsigned int i = 0; i < ncp; i++) {
		cw[i] = integrate(cubLagr[i], dimension, sizeFactor) / sizeFactor;
	}
}

template <int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildNodeIndices(
 CartesianVector<int,nsc> *res,
 const unsigned int order,
 const unsigned int nNodes) const {
	// Computes first coordinate indices vector.
	DynMatrix<int> ini(nNodes,nsc);
	for (unsigned int i = 0; i <= order; i++) {
		for (unsigned int j = numberOfNodes(i-1); j < nNodes; j++) {
			ini(j,0) = order - i;
		}
	}
	// Computes ordered nodal simplex coordinates.
	DynMatrix<int> ord(nNodes,nsc);
	for (unsigned int i = 0; i < nsc; i++) {
		ord = PMatrix(order,i) * ini;
		// Stores in nId matrix.
		for (unsigned int j = 0; j < nNodes; j++) {
			res[j](i) = ord(j,0);
		}
	}
}

 
template<int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildCubatureLagrange() {
	unsigned int i, j, k, c;
	// Evaluates Lagrange and Lagrange derived polynomials in cubature points.
	for (unsigned int i = 0; i < np; i++) {
		for (unsigned int c = 0; c < ncp; c++) {
			ca[i][c] = lagr[i].eval(cPos[c]);
		}
	}
	for (unsigned int i = 0; i < np; i++) {
		for (unsigned int j = 0; j < faces; j++) {
			for (unsigned int c = 0; c < ncp; c++) {
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
 
template <int SIMPTET_N>
inline unsigned int
SimplexTet<SIMPTET_N>::nodeIndex(const unsigned int i, const unsigned int j) const {
	return nId[i](j);
}

template <int SIMPTET_N>
inline unsigned int
SimplexTet<SIMPTET_N>::cubatureNodeIndex(const unsigned int i, const unsigned int j) const {
	return cId[i](j);
}

template <int SIMPTET_N>
inline const Polynomial<double>&
SimplexTet<SIMPTET_N>::getLagr(const unsigned int i) const {
	return lagr[i];
}

template <int SIMPTET_N>
inline const Polynomial<double>&
SimplexTet<SIMPTET_N>::getDLagr(const unsigned int i, const unsigned int f) const {
	return dLagr[i][f];
}

template <int SIMPTET_N>
double
SimplexTet<SIMPTET_N>::getCda(unsigned int i, unsigned int j, unsigned int k) const {
	return cda[i][j][k];
}
 
template <int SIMPTET_N>
StaMatrix<double, SIMPTET_NP, SIMPTET_NP>
SimplexTet<SIMPTET_N>::getMassMatrix() const {
    StaMatrix<double,np,np> res;
	for (unsigned int c = 0; c < ncp; c++) {
		res += cwaa[c];
	}
	return res;
}

template <int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildLIFTMatrix() {
	// Obtains mass matrix for the tetrahedron.
	StaMatrix<double,np,np> invM;
	invM = getMassMatrix().invert();
	// Obtains side mass matrix using simplex triangle.
    StaMatrix<double, nfp, nfp> sM;
	sM = tri.getMassMatrix();
	// Computes LIFT matrix.
	StaMatrix<int, np, nfp> RtrInt;
	StaMatrix<double, np, nfp> RtrDbl;
	for (unsigned int f = 0; f < faces; f++) {
		RtrInt = R[f].transpose();
		RtrDbl.assign(RtrInt);
		LIFT[f] = RtrDbl * sM;
		LIFT[f] = invM * LIFT[f];
	}
}
 
template <int SIMPTET_N>
inline unsigned int
SimplexTet<SIMPTET_N>::vertex(const unsigned int vertexNum) const {
	return sideNode(vertexNum,0);
}
 
template <int SIMPTET_N>
inline unsigned int
SimplexTet<SIMPTET_N>::sideVertex(
 const unsigned int face, const unsigned int vertexNum) const {
	return sideNode(face, tri.vertex(vertexNum));
}
 
template <int SIMPTET_N>
inline unsigned int
SimplexTet<SIMPTET_N>::sideNode(
 const unsigned int face, const unsigned int num) const {
	return sNId(face, num);
}
 
template <int SIMPTET_N>
double
SimplexTet<SIMPTET_N>::integrateScalarsOnFace(
 const double x[SIMPTET_NP],
 const unsigned int f,
 const double area) const {
	// Takes an array of np scalars, a face number f and an area.
	// Returns a vector that is the result of integrating over the face of
	// the simplex and multiply the result by the area.
	double faceX[nfp];
    for (unsigned int i = 0; i < nfp; i++)
    	faceX[i] = x[sideNode(f,i)];
    // Computes result.
    double res;
    for (unsigned int i = 0; i < nfp; i++) {
    	for (unsigned int c = 0; c < ncp; c++) {
    		res += tri.cw[c] * faceX[i];
    	}
    }
    res *= area;
    cerr << "ERROR@SimplexTet::integrateScalarsOnFace" << endl;
    cerr << "This has to be redone." << endl;
    exit(SIMPLEX_ERROR);
	return res;
}
 
template <int SIMPTET_N>
double
SimplexTet<SIMPTET_N>::integrateScalars(
  const double x[SIMPTET_NP],
  const double volume) const {
	double res = 0.0;
    for (unsigned int i = 0; i < np; i++) {
    	for (unsigned int c = 0; c < ncp; c++) {
    		res += cw[c] * ca[i][c] * x[i];
    	}
    }
    return (res*volume);
}

template <int SIMPTET_N>
CartesianVector<double,4>
SimplexTet<SIMPTET_N>::coordinate(const unsigned int i) const {
	CartesianVector<double,4> res;
	res = nId[i];
	res /= (double) n;
	return res;
}

template <int SIMPTET_N>
CartesianVector<double,4>
SimplexTet<SIMPTET_N>::sideCoordinate(
 const unsigned int f, const unsigned int i) const {
	return coordinate(sideNode(f,i));
}
 
template <int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::buildSideNodeIndices() {
	StaMatrix<int,np,1> nList;
	StaMatrix<int,nfp,1> aux;
	// Initializes node list.
	for (unsigned int i = 0; i < np; i++) {
		nList(i,0) = i;
	}
	// Creates aux matrix to store the computed sNId.
	for (unsigned int f = 0; f < faces; f++) {
		aux = R[f] * nList;
		for (unsigned int i = 0; i < nfp; i++)
			sNId(f,i) = aux(i,0);
	}
}
 
template<int SIMPTET_N>
int
SimplexTet<SIMPTET_N>::numberOfNodes(int order) const {
	int res = 1;
	for (int i = 1; i < int(nsc); i++)
		res *= (order + i);
	res /= factorial(nsc - 1);
	return res;
}
 
template <int SIMPTET_N>
DynMatrix<int>
SimplexTet<SIMPTET_N>::PMatrix(const unsigned int n, const unsigned int s) const {
	// Purpose: Creates indices rotation matrix for a simplex tetrahedral
	//          element in 3-D.
	unsigned int np = numberOfNodes(n);
	DynMatrix<int> res(np,np);
	if (s == 0) {
		res.eye();
	} else if (s >= 1 && s <= 3) {
		// P2Dall will store the rotation matrix for all the indices slices.
		DynMatrix<int> P2Dall(np,np);
		unsigned int nodesSet = 0;
		for (unsigned int i = 0; i <= n; i++) {
			unsigned int sliceNP = (i + 1) * (i + 2) / 2;
			// Assign P2D to P2Dall matrix.
			DynMatrix<int> auxP = tri.PMatrix(i,2);
			for (unsigned int j = 0; j < sliceNP; j++) {
				for (unsigned int k = 0; k < sliceNP; k++) {
					P2Dall(j+nodesSet,k+nodesSet) = auxP.val(j,k);
				}
			}
			nodesSet += sliceNP;
		}
		// Allocates and initializes ordered and final indices vectors.
		DynMatrix<int> orVec(np,1), fiVec(np,1), temp(np,1);
		for (unsigned int i = 0; i < np; i++) {
			orVec(i,0) = i;
		}
		// Computes final indices positions.
		fiVec = P2Dall * orVec;
		// Assigns ones to the Q1 matrix.
		DynMatrix<int> Q[faces];
		DynMatrix<int> QInit(np,np);
		for (unsigned int i = 0; i < faces; i++) {
			Q[i] = QInit;
		}
		for (unsigned int i = 0; i < np; i++) {
			Q[0](orVec(i,0),fiVec(i,0)) = 1;
		}
		// Defines Q[1], same as Q[0] but around vertex 1.
		// Assigns nodes to the original (non-rotated) enumeration vector.
		orVec.zeros();
		unsigned int lastSet = n * (n+1) * (n+2) / 6 + 1;
		unsigned int lastNodeSet = 0;
		for (unsigned int i = 0; i <= n; i++)
			for (unsigned int j = 1; j <= i+1; j++) {
				orVec(lastNodeSet+j-1,0) = lastSet;
				lastSet += 1;
				lastNodeSet += j;
			}
		lastSet = n * (n+1) * (n+2) / 6 + 1;
		for (unsigned int j = 1; j <= n; j++) {
			temp = orVec;
			for(int i = int(np); i >= 2; i--) {
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
		for (unsigned int i = 0; i < np; i++) {
			Q[1](orVec(i,0)-1,fiVec(i,0)-1) = 1;
		}
		DynMatrix<int> Q0Sq, Q1Sq;
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
 
template <int SIMPTET_N>
StaMatrix<int,SIMPTET_NFP,SIMPTET_NP>
SimplexTet<SIMPTET_N>::RMatrix(const unsigned int s) const {
	// Creates node indices vector with node indices on face 1.
	unsigned int last = 0;
	StaMatrix<int,SIMPTET_NFP,1> nodeVec;
	for (unsigned int i = 1; i <= n + 1; i++) {
		unsigned int nsp = (i-1) * (i) * (i+1) / 6;
		for (unsigned int j = 0; j<i; j++) {
			nodeVec(last + j,0) = nsp + (j)*(j+1)/2 + 1;
		}
		last += i;
	}
	// Creates extraction matrix R for face 1.
	StaMatrix<int,SIMPTET_NFP,SIMPTET_NP> Raux;
	for (unsigned int i = 0; i < nfp; i++) {
		Raux(i,nodeVec(i,0)-1) = 1;
	}
	// Returns final form of R.
	StaMatrix<int,SIMPTET_NFP,SIMPTET_NP> res = Raux * P[s];
	return res;
}

template <int SIMPTET_N>
void
SimplexTet<SIMPTET_N>::printInfo() const {
	cout << " --- SimplexTet Information --- " << endl;
	cout << " Number of coordinates:         " << faces << endl;
	cout << " Order:                         " << n << endl;
	cout << " Number of nodes:               " << np << endl;
	cout << " Number of face nodes:          " << nfp << endl;
	cout << " Rotation matrices:             " << endl;
	for (unsigned int i = 0; i < faces; i++) {
		cout << "Rotation around simplex coordinate #" << i << endl;
		P[i].printInfo();
	}
	cout << " Extraction matrices:           " << endl;
	for (unsigned int i = 0; i < faces; i++) {
		cout << "Extraction matrices for face #" << i << endl;
		R[i].printInfo();
	}
	cout << " List of node indices: " << endl;
	for (unsigned int i = 0; i < np; i++) {
		nId[i].printInfo();
		cout << endl;
	}
	cout << " List of node coordinates: " << endl;
	for (unsigned int i = 0; i < np; i++) {
		coordinate(i).printInfo();
		cout << endl;
	}
	cout << " List of side nodes indices:    " << endl;
	sNId.printInfo();
	cout << " List of side nodes coordinates: " << endl;
	for (unsigned int f = 0; f < faces; f++) {
		cout << "Face #" << f << endl;
		for (unsigned int i = 0; i < nfp; i++) {
			sideCoordinate(f,i).printInfo();
			cout << endl;
		}
	}
	cout << " Lagrange polynomials:          " << endl;
	for (unsigned int i = 0; i < np; i++) {
		cout << "Lagrange polynomial of node #" << i << endl;
		lagr[i].printInfo();
	}
	cout << " Lagrange polynomials derivatives: " << endl;
	for (unsigned int i = 0; i < np; i++)
		for (unsigned int j = 0; j < faces; j++) {
			cout << "Pol. " << i << " derived w.r.t. var." << j << endl;
			dLagr[i][j].printInfo();
		}
	cout << " LIFT Matrices" << endl;
	for (unsigned int i = 0; i < faces; i++)
		LIFT[i].printInfo();
	cout << " Order of cubature: " << nc << endl;
	cout << " Number of cub. points: " << ncp << endl;
	cout << " Cubature positions and weights: " << endl;
	for (unsigned int i = 0; i < ncp; i++) {
		cout << "#" << i << " ";
		cPos[i].printInfo();
		cout << " " << cw[i] << endl;
	}
	double sum = 0.0;
	for (unsigned int i = 0; i < ncp; i++) {
		sum += cw[i];
	}
	cout << " The sum of cubature weights is: " << sum << endl;
	cout << " --- End of simplex information --- "   << endl;
}
