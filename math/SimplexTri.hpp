#ifndef SIMPLEXTRI_H_
	#include "SimplexTri.h"
#endif
// =============== SimplexTri methods =========================================
template <int SIMPTRI_N>
SimplexTri<SIMPTRI_N>::SimplexTri() {
	for (unsigned int s = 0; s < nsc; s++) {
		P[s] = PMatrix(n,s);
	}
	buildNodeIndices(nId, n, np);
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
	buildCubaturePositionsAndWeights();
	buildCubatureLagrange();
}

 
template <int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::buildCubaturePositionsAndWeights() {
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
 
template <int SIMPTRI_N>
StaMatrix<double, SIMPTRI_NP, SIMPTRI_NP>
SimplexTri<SIMPTRI_N>::getMassMatrix() const {
	StaMatrix<double,np,np> res;
	for (unsigned int c = 0; c < ncp; c++) {
		res += cwaa[c];
	}
	return res;
}
 
template<int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::buildCubatureLagrange() {
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

template <int SIMPTRI_N>
inline unsigned int
SimplexTri<SIMPTRI_N>::nodeIndex(const unsigned int i, const unsigned int j) const {
	return nId[i](j);
}

template <int SIMPTRI_N>
inline unsigned int
SimplexTri<SIMPTRI_N>::cubatureNodeIndex(unsigned int i, unsigned int j) const {
	return cId[i](j);
}

template <int SIMPTRI_N>
const Polynomial<double>&
SimplexTri<SIMPTRI_N>::getLagr(const unsigned int i) const {
	return lagr[i];
}

template <int SIMPTRI_N>
const Polynomial<double>&
SimplexTri<SIMPTRI_N>::getDLagr(const unsigned int i, const unsigned int f) const {
	return dLagr[i][f];
}

template<int SIMPTRI_N>
inline unsigned int
SimplexTri<SIMPTRI_N>::vertex(unsigned int i) const {
	return sideNode(i,0);
}

template<int SIMPTRI_N>
inline unsigned int
SimplexTri<SIMPTRI_N>::sideVertex(const unsigned int f, const unsigned int i) const {
	return sideNode(f,i);
}

template <int SIMPTRI_N>
inline unsigned int
SimplexTri<SIMPTRI_N>::sideNode(
 const unsigned int face, const unsigned int num) const {
	return sNId(face, num);
}

template <int SIMPTRI_N>
CartesianVector<double,3>
SimplexTri<SIMPTRI_N>::coordinate(const unsigned int i) const {
	CartesianVector<double,3> res;
	res = nId[i];
	res /= (double) n;
	return res;
}

template <int SIMPTRI_N>
unsigned int
SimplexTri<SIMPTRI_N>::numberOfNodes(const unsigned int order) const {
	int res = 1;
	for (unsigned int i = 1; i < nsc; i++)
		res *= (order + i);
	res /= factorial(nsc-1);
	return res;
}
 
template <int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::buildSideNodeIndices() {
	StaMatrix<int,np,1> nList;
	StaMatrix<int,nfp,1> aux;
	// Initializes node list.
	for (unsigned int i = 0; i < np; i++)
		nList(i,0) = i;
	// Creates aux matrix to store the computed sNId.
	for (unsigned int f = 0; f < faces; f++) {
		aux = R[f] * nList;
		for (unsigned int i = 0; i < nfp; i++)
			sNId(f,i) = aux(i,0);
	}
}
 
template <int SIMPTRI_N>
DynMatrix<int>
SimplexTri<SIMPTRI_N>::PMatrix(unsigned int order, unsigned int s) const {
	// Creates indices rotation matrix for a simplex triangle element.
	// Rotation is clockwise.
	unsigned int np = numberOfNodes(order);
	unsigned int nfp = order + 1;
	DynMatrix<int> res(np,np);
	DynMatrix<int> original(nfp,nfp), rotated(nfp,nfp);
	// Creates original numeration matrix.
	unsigned int originalNum = 1;
	for (unsigned int i = 0; i < nfp; i++)
		for (unsigned int j = 0; j <= i; j++) {
			original(i,j) = originalNum;
			originalNum++;
		}
	// Creates rotated numeration matrix.
	unsigned int rotatedNum = 1;
	if (s == 0) {
		res.eye();
	} else if (s == 1 || s == 2) {
		for (unsigned int i = 0; i <= nfp; i++) {
			int j = int(nfp - 1);
			while (j >= int(nfp - i)) {
				rotated(j,j-nfp+i) = rotatedNum;
				rotatedNum = rotatedNum + 1;
				j--;
			}
		}
		// Assigns values to P matrix.
		for (unsigned int i = 0; i < nfp; i++) {
			for (unsigned int j = 0; j <= i; j++) {
				res(rotated(i,j)-1,original(i,j)-1) = 1;
			}
		}
		if (s == 1)
			res = res * res;
	}
	return res;
}

template <int SIMPTRI_N>
StaMatrix<int, SIMPTRI_NFP, SIMPTRI_NP>
SimplexTri<SIMPTRI_N>::RMatrix(const unsigned int s) const {
	// Creates node indices vector with node indices on face 1.
	StaMatrix<int,nfp,1> nodeVec;
	unsigned int last = 0;
	for (unsigned int i = 0; i < n + 1; i++) {
		last += i;
		nodeVec(i,0) = last;
	}
	// Creates extraction matrix R for face 1.
	StaMatrix<int,nfp,np> Raux;
	for (unsigned int i = 0; i < nfp; i++)
		Raux(i,nodeVec(i,0)) = 1;
	// Returns final form of R.
	StaMatrix<int,nfp,np> res = Raux * P[s] * P[s];
	return res;
}
 
//template <int SIMPTRI_N>
//StaMatrix<double, SIMPTRI_NP, SIMPTRI_NP>
//SimplexTri<SIMPTRI_N>::getMassMatrix() const {
//	vector<StaMatrix<double,np,np> > cwaa;
//	cwaa = cubatureLagrangeProducts();
//	StaMatrix<double,np,np> res;
//	for (unsigned int c = 0; c < cwaa.size(); c++) {
//		res += cwaa[c];
//	}
//	return res;
//}
 
//template <int SIMPTRI_N> inline
//vector<vector<double> >
//SimplexTri<SIMPTRI_N>::cubatureLagrange() const {
//	// PURPOSE:
//	// - Evaluates Lagrange and Lagrange derived polynomials in cubature
//	//   points.
//	TriangleRule triRule(3);
//	vector<Evaluation> ca(np, Evaluation(triRule.ncp));
//	for (unsigned int i = 0; i < np; i++)
//		ca[i] = triRule.eval(lagr[i]);
//	return ca;
//}
// 
//template <int SIMPTRI_N> inline
//vector<StaMatrix<double, SIMPTRI_NP, SIMPTRI_NP> >
//SimplexTri<SIMPTRI_N>::cubatureLagrangeProducts() const {
//	// ========================================================================
//	// PURPOSE:
//	// - Gets cubatured alpha and derived alpha for simplex element
//	unsigned int i, j, c;
//	TriangleRule triRule(3);
//	StaMatrix<double, np, np> zero;
//	vector<StaMatrix<double, np, np> > res(triRule.ncp,  zero);
//	vector<Evaluation> ca(np, Evaluation(triRule.ncp));
//	ca = cubatureLagrange();
//	// Computes Cubature weighted alpha by alpha products.
//	for (c = 0; c < triRule.ncp; c++)
//		for (i = 0; i < np; i++)
//			for (j = 0; j < np; j++)
//				res[c](i,j) = triRule.w[c] * ca[i].val[c] * ca[j].val[c];
//	return res;
//}
// 
//template <int SIMPTRI_N> inline
//void
//SimplexTri<SIMPTRI_N>::computeWeights() {
//	for (unsigned int i = 0; i < np; i++)
//		weight[i] = lagr[i].integrateInSimplexSpace();
//}
 
template <int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::buildNodeIndices(
 CartesianVector<int,nsc> *res,
 const unsigned int order,
 const unsigned int nNodes) const {
	// Computes first coordinate indices vector.
	DynMatrix<int> ini(nNodes,nsc);
	for (unsigned int i = 0; i <= order; i++)
		for (unsigned int j = numberOfNodes(i-1); j < nNodes; j++)
			ini(j,0) = order - i;
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

template <int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::printInfo() const {
	cout << " --- SimplexTri Information --- "   << endl;
	cout << " Order:                         " <<   n << endl;
	cout << " Number of nodes:               " <<  np << endl;
	cout << " Number of face nodes:          " << nfp << endl;
	cout << " Number of coordinates:         " << nsc << endl;
	cout << " Rotation matrices:             "        << endl;
	for (unsigned int i = 0; i < nsc; i++) {
		cout << "Rotation around simplex coordinate #" << i << endl;
		P[i].printInfo();
	}
	cout << " List of node indices:          "        << endl;
	for (unsigned int i = 0; i < np; i++) {
		nId[i].printInfo();
		cout << endl;
	}
	cout << " Extraction matrices:           "        << endl;
	for (unsigned int i = 0; i < faces; i++) {
		cout << "Extraction matrices for face #" << i << endl;
		R[i].printInfo();
	}
	cout << " List of side nodes indices:    "        << endl;
	sNId.printInfo();
	cout << " Lagrange polynomials:          "        << endl;
	for (unsigned int i = 0; i < np; i++) {
		cout << "Lagrange polynomial of node #" <<  i << endl;
		lagr[i].printInfo();
	}
	cout << " Vertices:" << endl;
	for (unsigned int i = 0; i < vertices; i++)
		cout << "Vertex #" << i << ": " << vertex(i) << endl;
//	cout << " Weights:                       "       << endl;
//	double totalWeight = 0.0;
//	for (unsigned int i = 0; i < np; i++) {
//		cout << weight[i] << endl;
//		totalWeight += weight[i];
//	}
//	cout << " Weights sum: " << totalWeight          << endl;
	cout << " Order of cubature: " << nc << endl;
	cout << " Number of cub. points: " << ncp << endl;
	cout << " Cubature positions and weights: " << endl;
	for (unsigned int i = 0; i < ncp; i++) {
		cout << "#" << i << " ";
		cPos[i].printInfo();
		cout << " " << cw[i] << endl;
	}
	cout << " --- End of simplex information --- "   << endl;
}
