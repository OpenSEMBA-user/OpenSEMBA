#ifndef SIMPLEXTRI_H_
	#include "SimplexTri.h"
#endif
// =============== SimplexTri methods =========================================
template <Int SIMPTRI_N>
SimplexTri<SIMPTRI_N>::SimplexTri() {
	for (UInt s = 0; s < nsc; s++) {
		P[s] = PMatrix(n,s);
	}
	buildNodeIndices(nId, n, np);
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
	buildCubaturePositionsAndWeights();
	buildCubatureLagrange();
}

 
template <Int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::buildCubaturePositionsAndWeights() {
	buildNodeIndices(cId,nc,ncp);
	CartesianVector<Real,nsc> aux;
	for (UInt i = 0; i < ncp; i++) {
		aux = cId[i];
		cPos[i] = aux / (Real) nc;
	}
	FunctionPolynomial<Real> cubLagr[ncp];
	cubatureLagrangePolynomials(cubLagr,nc,ncp,nsc);
	for (UInt i = 0; i < ncp; i++) {
		cw[i] = integrate(cubLagr[i], dimension, sizeFactor) / sizeFactor;
	}
}
 
template <Int SIMPTRI_N>
StaMatrix<Real, SIMPTRI_NP, SIMPTRI_NP>
SimplexTri<SIMPTRI_N>::getMassMatrix() const {
	StaMatrix<Real,np,np> res;
	for (UInt c = 0; c < ncp; c++) {
		res += cwaa[c];
	}
	return res;
}
 
template<Int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::buildCubatureLagrange() {
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

template <Int SIMPTRI_N>
inline UInt
SimplexTri<SIMPTRI_N>::nodeIndex(const UInt i, const UInt j) const {
	return nId[i](j);
}

template <Int SIMPTRI_N>
inline UInt
SimplexTri<SIMPTRI_N>::cubatureNodeIndex(UInt i, UInt j) const {
	return cId[i](j);
}

template <Int SIMPTRI_N>
const FunctionPolynomial<Real>&
SimplexTri<SIMPTRI_N>::getLagr(const UInt i) const {
	return lagr[i];
}

template <Int SIMPTRI_N>
const FunctionPolynomial<Real>&
SimplexTri<SIMPTRI_N>::getDLagr(const UInt i, const UInt f) const {
	return dLagr[i][f];
}

template<Int SIMPTRI_N>
inline UInt
SimplexTri<SIMPTRI_N>::vertex(UInt i) const {
	return sideNode(i,0);
}

template<Int SIMPTRI_N>
inline UInt
SimplexTri<SIMPTRI_N>::sideVertex(const UInt f, const UInt i) const {
	return sideNode(f,i);
}

template <Int SIMPTRI_N>
inline UInt
SimplexTri<SIMPTRI_N>::sideNode(
 const UInt face, const UInt num) const {
	return sNId(face, num);
}

template <Int SIMPTRI_N>
CartesianVector<Real,3>
SimplexTri<SIMPTRI_N>::coordinate(const UInt i) const {
	CartesianVector<Real,3> res;
	res = nId[i];
	res /= (Real) n;
	return res;
}

template <Int SIMPTRI_N>
UInt
SimplexTri<SIMPTRI_N>::numberOfNodes(const UInt order) const {
	Int res = 1;
	for (UInt i = 1; i < nsc; i++)
		res *= (order + i);
	res /= factorial(nsc-1);
	return res;
}
 
template <Int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::buildSideNodeIndices() {
	StaMatrix<Int,np,1> nList;
	StaMatrix<Int,nfp,1> aux;
	// Initializes node list.
	for (UInt i = 0; i < np; i++)
		nList(i,0) = i;
	// Creates aux matrix to store the computed sNId.
	for (UInt f = 0; f < faces; f++) {
		aux = R[f] * nList;
		for (UInt i = 0; i < nfp; i++)
			sNId(f,i) = aux(i,0);
	}
}
 
template <Int SIMPTRI_N>
DynMatrix<Int>
SimplexTri<SIMPTRI_N>::PMatrix(UInt order, UInt s) const {
	// Creates indices rotation matrix for a simplex triangle element.
	// Rotation is clockwise.
	UInt np = numberOfNodes(order);
	UInt nfp = order + 1;
	DynMatrix<Int> res(np,np);
	DynMatrix<Int> original(nfp,nfp), rotated(nfp,nfp);
	// Creates original numeration matrix.
	UInt originalNum = 1;
	for (UInt i = 0; i < nfp; i++)
		for (UInt j = 0; j <= i; j++) {
			original(i,j) = originalNum;
			originalNum++;
		}
	// Creates rotated numeration matrix.
	UInt rotatedNum = 1;
	if (s == 0) {
		res.eye();
	} else if (s == 1 || s == 2) {
		for (UInt i = 0; i <= nfp; i++) {
			Int j = Int(nfp - 1);
			while (j >= Int(nfp - i)) {
				rotated(j,j-nfp+i) = rotatedNum;
				rotatedNum = rotatedNum + 1;
				j--;
			}
		}
		// Assigns values to P matrix.
		for (UInt i = 0; i < nfp; i++) {
			for (UInt j = 0; j <= i; j++) {
				res(rotated(i,j)-1,original(i,j)-1) = 1;
			}
		}
		if (s == 1)
			res = res * res;
	}
	return res;
}

template <Int SIMPTRI_N>
StaMatrix<Int, SIMPTRI_NFP, SIMPTRI_NP>
SimplexTri<SIMPTRI_N>::RMatrix(const UInt s) const {
	// Creates node indices vector with node indices on face 1.
	StaMatrix<Int,nfp,1> nodeVec;
	UInt last = 0;
	for (UInt i = 0; i < n + 1; i++) {
		last += i;
		nodeVec(i,0) = last;
	}
	// Creates extraction matrix R for face 1.
	StaMatrix<Int,nfp,np> Raux;
	for (UInt i = 0; i < nfp; i++)
		Raux(i,nodeVec(i,0)) = 1;
	// Returns final form of R.
	StaMatrix<Int,nfp,np> res = Raux * P[s] * P[s];
	return res;
}
 
//template <Int SIMPTRI_N>
//StaMatrix<Real, SIMPTRI_NP, SIMPTRI_NP>
//SimplexTri<SIMPTRI_N>::getMassMatrix() const {
//	vector<StaMatrix<Real,np,np> > cwaa;
//	cwaa = cubatureLagrangeProducts();
//	StaMatrix<Real,np,np> res;
//	for (UInt c = 0; c < cwaa.size(); c++) {
//		res += cwaa[c];
//	}
//	return res;
//}
 
//template <Int SIMPTRI_N> inline
//vector<vector<Real> >
//SimplexTri<SIMPTRI_N>::cubatureLagrange() const {
//	// PURPOSE:
//	// - Evaluates Lagrange and Lagrange derived polynomials in cubature
//	//   points.
//	TriangleRule triRule(3);
//	vector<Evaluation> ca(np, Evaluation(triRule.ncp));
//	for (UInt i = 0; i < np; i++)
//		ca[i] = triRule.eval(lagr[i]);
//	return ca;
//}
// 
//template <Int SIMPTRI_N> inline
//vector<StaMatrix<Real, SIMPTRI_NP, SIMPTRI_NP> >
//SimplexTri<SIMPTRI_N>::cubatureLagrangeProducts() const {
//	// ========================================================================
//	// PURPOSE:
//	// - Gets cubatured alpha and derived alpha for simplex element
//	UInt i, j, c;
//	TriangleRule triRule(3);
//	StaMatrix<Real, np, np> zero;
//	vector<StaMatrix<Real, np, np> > res(triRule.ncp,  zero);
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
//template <Int SIMPTRI_N> inline
//void
//SimplexTri<SIMPTRI_N>::computeWeights() {
//	for (UInt i = 0; i < np; i++)
//		weight[i] = lagr[i].integrateInSimplexSpace();
//}
 
template <Int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::buildNodeIndices(
 CartesianVector<Int,nsc> *res,
 const UInt order,
 const UInt nNodes) const {
	// Computes first coordinate indices vector.
	DynMatrix<Int> ini(nNodes,nsc);
	for (UInt i = 0; i <= order; i++)
		for (UInt j = numberOfNodes(UInt(i-1)); j < nNodes; j++)
			ini(j,0) = order - i;
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

template <Int SIMPTRI_N>
void
SimplexTri<SIMPTRI_N>::printInfo() const {
	cout << " --- SimplexTri Information --- "   << endl;
	cout << " Order:                         " <<   n << endl;
	cout << " Number of nodes:               " <<  np << endl;
	cout << " Number of face nodes:          " << nfp << endl;
	cout << " Number of coordinates:         " << nsc << endl;
	cout << " Rotation matrices:             "        << endl;
	for (UInt i = 0; i < nsc; i++) {
		cout << "Rotation around simplex coordinate #" << i << endl;
		P[i].printInfo();
	}
	cout << " List of node indices:          "        << endl;
	for (UInt i = 0; i < np; i++) {
		nId[i].printInfo();
		cout << endl;
	}
	cout << " Extraction matrices:           "        << endl;
	for (UInt i = 0; i < faces; i++) {
		cout << "Extraction matrices for face #" << i << endl;
		R[i].printInfo();
	}
	cout << " List of side nodes indices:    "        << endl;
	sNId.printInfo();
	cout << " Lagrange polynomials:          "        << endl;
	for (UInt i = 0; i < np; i++) {
		cout << "Lagrange polynomial of node #" <<  i << endl;
		lagr[i].printInfo();
	}
	cout << " Vertices:" << endl;
	for (UInt i = 0; i < vertices; i++)
		cout << "Vertex #" << i << ": " << vertex(i) << endl;
//	cout << " Weights:                       "       << endl;
//	Real totalWeight = 0.0;
//	for (UInt i = 0; i < np; i++) {
//		cout << weight[i] << endl;
//		totalWeight += weight[i];
//	}
//	cout << " Weights sum: " << totalWeight          << endl;
	cout << " Order of cubature: " << nc << endl;
	cout << " Number of cub. points: " << ncp << endl;
	cout << " Cubature positions and weights: " << endl;
	for (UInt i = 0; i < ncp; i++) {
		cout << "#" << i << " ";
		cPos[i].printInfo();
		cout << " " << cw[i] << endl;
	}
	cout << " --- End of simplex information --- "   << endl;
}
