#ifndef SIMPLEXLIN_H_
	#include "SimplexLin.h"
#endif

// =============== SimplexLin =================================================
// =-=-=-=-=-=-=-= Constructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template <Int SIMPLIN_N>
SimplexLin<SIMPLIN_N>::SimplexLin() {
	// ---------- Matrices and indices ----------------------------------------
	/*for (Int s = 0; s < nsc; s++) {
		P[s] = PMatrix(n,s);
	}*/
	buildNodeIndices(nId, n, np);
	for (UInt s = 0; s < nsc; s++) {
		R[s] = RMatrix(s);
	}
	buildSideNodeIndices();
	// ----------- Lagrange polynomials ---------------------------------------
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
};
 
template <Int SIMPLIN_N>
inline UInt
SimplexLin<SIMPLIN_N>::nodeIndex(const UInt i, const UInt j) const {
	return nId[i](j);
}
 
template <Int SIMPLIN_N>
inline UInt
SimplexLin<SIMPLIN_N>::cubatureNodeIndex(UInt i, UInt j) const {
	return cId[i](j);
}
 
template <Int SIMPLIN_N>
inline UInt
SimplexLin<SIMPLIN_N>::vertex(const UInt vertexNum) const {
	return sideNode(vertexNum,0);
}
 
template <Int SIMPLIN_N>
inline UInt
SimplexLin<SIMPLIN_N>::sideNode(
 const UInt face, const UInt num) const {
	return sNId(face, num);
}
 
template <Int SIMPLIN_N>
const Polynomial<Real>&
SimplexLin<SIMPLIN_N>::getLagr(const UInt i) const {
	return lagr[i];
}
 
template <Int SIMPLIN_N>
const Polynomial<Real>&
SimplexLin<SIMPLIN_N>::getDLagr(const UInt i, const UInt f) const {
	return dLagr[i][f];
}
 
template <Int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::buildNodeIndices(
 CartesianVector<Int,nsc> *res,
 const UInt order,
 const UInt nNodes) const {
	// Computes first coordinate indices vector.
	for (UInt i = 0; i < nNodes; i++) {
		res[i](0) = order - i;
		res[i](1) = i;
	}
}
 
template <Int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::buildSideNodeIndices() {
	StaMatrix<Int,np,1> nList;
	StaMatrix<Int,nfp,1> aux;
	// Initializes node list.
	for (UInt i = 0; i < np; i++)
		nList(i,0) = i;
	// Creates aux matrix to store the computed sNId.
	for (UInt f = 0; f < faces; f++) {
		aux = R[f] * nList;
		for (UInt i = 0; i < nfp; i++) {
			sNId(f,i) = aux(i,0);
		}
	}
}
 
template <Int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::buildCubaturePositionsAndWeights() {
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
 
template<Int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::buildCubatureLagrange() {
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
 
template<Int SIMPLIN_N>
UInt
SimplexLin<SIMPLIN_N>::numberOfNodes(const UInt order) const {
	return (order + 1);
}
 
template <Int SIMPLIN_N>
DynMatrix<Int>
SimplexLin<SIMPLIN_N>::PMatrix(const UInt n, const UInt s) const {
	UInt np = numberOfNodes(n);
	DynMatrix<Int> res(np,np);
	if (s == 0) {
		res.eye();
	} else  {
		res.zeros();
		for (UInt i = 0; i < np; i++) {
			res(i, np-i-1) = (Int) 1;
		}
	}
	return res;
}
 
template <Int SIMPLIN_N>
StaMatrix<Int,SIMPLIN_NFP,SIMPLIN_NP>
SimplexLin<SIMPLIN_N>::RMatrix(const UInt s) const {
	// Creates extraction matrix R for face 1.
	StaMatrix<Int,nfp,np> Raux;
	Raux.zeros();
	Raux(0,0) = (Int) 1;
	StaMatrix<Int,nfp,np> res = Raux * P[s];
	return res;
}
 
template <Int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::printInfo() const {
	cout << " --- SimplexLin Information --- " << endl;
	cout << " Number of coordinates:         " << nsc << endl;
	cout << " Order:                         " << n << endl;
	cout << " Number of nodes:               " << np << endl;
	cout << " Number of face nodes:          " << nfp << endl;
	cout << " Order of cubature integration: " << nc << endl;
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
	cout << " List of node indices:          " << endl;
	for (UInt i = 0; i < np; i++) {
		nId[i].printInfo();
		cout << endl;
	}
	cout << " List of side nodes indices:    " << endl;
	sNId.printInfo();
	cout << " Lagrange polynomials:          " << endl;
	for (UInt i = 0; i < np; i++) {
		cout << "Lagrange polynomial of node #" <<  i << endl;
		lagr[i].printInfo();
	}
	cout << " Lagrange polynomials derivatives: " << endl;
	for (UInt i = 0; i < np; i++) {
		for (UInt j = 0; j < faces; j++) {
			cout << "Pol. " << i << " derived w.r.t. var." << j << endl;
			dLagr[i][j].printInfo();
		}
	}
	cout << " Cubature positions and weights: " << endl;
	for (UInt i = 0; i < ncp; i++) {
		cout << "#" << i << " ";
		cPos[i].printInfo();
		cout << " " << cw[i] << endl;
	}
	cout << " --- End of simplex information --- " << endl;
}
