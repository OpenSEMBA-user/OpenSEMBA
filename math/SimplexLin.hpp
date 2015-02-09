#ifndef SIMPLEXLIN_H_
	#include "SimplexLin.h"
#endif

// =============== SimplexLin =================================================
// =-=-=-=-=-=-=-= Constructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template <int SIMPLIN_N>
SimplexLin<SIMPLIN_N>::SimplexLin() {
	// ---------- Matrices and indices ----------------------------------------
	/*for (int s = 0; s < nsc; s++) {
		P[s] = PMatrix(n,s);
	}*/
	buildNodeIndices(nId, n, np);
	for (unsigned int s = 0; s < nsc; s++) {
		R[s] = RMatrix(s);
	}
	buildSideNodeIndices();
	// ----------- Lagrange polynomials ---------------------------------------
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
};
 
template <int SIMPLIN_N>
inline unsigned int
SimplexLin<SIMPLIN_N>::nodeIndex(const unsigned int i, const unsigned int j) const {
	return nId[i](j);
}
 
template <int SIMPLIN_N>
inline unsigned int
SimplexLin<SIMPLIN_N>::cubatureNodeIndex(unsigned int i, unsigned int j) const {
	return cId[i](j);
}
 
template <int SIMPLIN_N>
inline unsigned int
SimplexLin<SIMPLIN_N>::vertex(const unsigned int vertexNum) const {
	return sideNode(vertexNum,0);
}
 
template <int SIMPLIN_N>
inline unsigned int
SimplexLin<SIMPLIN_N>::sideNode(
 const unsigned int face, const unsigned int num) const {
	return sNId(face, num);
}
 
template <int SIMPLIN_N>
const Polynomial<double>&
SimplexLin<SIMPLIN_N>::getLagr(const unsigned int i) const {
	return lagr[i];
}
 
template <int SIMPLIN_N>
const Polynomial<double>&
SimplexLin<SIMPLIN_N>::getDLagr(const unsigned int i, const unsigned int f) const {
	return dLagr[i][f];
}
 
template <int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::buildNodeIndices(
 CartesianVector<int,nsc> *res,
 const unsigned int order,
 const unsigned int nNodes) const {
	// Computes first coordinate indices vector.
	for (unsigned int i = 0; i < nNodes; i++) {
		res[i](0) = order - i;
		res[i](1) = i;
	}
}
 
template <int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::buildSideNodeIndices() {
	StaMatrix<int,np,1> nList;
	StaMatrix<int,nfp,1> aux;
	// Initializes node list.
	for (unsigned int i = 0; i < np; i++)
		nList(i,0) = i;
	// Creates aux matrix to store the computed sNId.
	for (unsigned int f = 0; f < faces; f++) {
		aux = R[f] * nList;
		for (unsigned int i = 0; i < nfp; i++) {
			sNId(f,i) = aux(i,0);
		}
	}
}
 
template <int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::buildCubaturePositionsAndWeights() {
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
 
template<int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::buildCubatureLagrange() {
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
 
template<int SIMPLIN_N>
unsigned int
SimplexLin<SIMPLIN_N>::numberOfNodes(const unsigned int order) const {
	return (order + 1);
}
 
template <int SIMPLIN_N>
DynMatrix<int>
SimplexLin<SIMPLIN_N>::PMatrix(const unsigned int n, const unsigned int s) const {
	unsigned int np = numberOfNodes(n);
	DynMatrix<int> res(np,np);
	if (s == 0) {
		res.eye();
	} else  {
		res.zeros();
		for (unsigned int i = 0; i < np; i++) {
			res(i, np-i-1) = (int) 1;
		}
	}
	return res;
}
 
template <int SIMPLIN_N>
StaMatrix<int,SIMPLIN_NFP,SIMPLIN_NP>
SimplexLin<SIMPLIN_N>::RMatrix(const unsigned int s) const {
	// Creates extraction matrix R for face 1.
	StaMatrix<int,nfp,np> Raux;
	Raux.zeros();
	Raux(0,0) = (int) 1;
	StaMatrix<int,nfp,np> res = Raux * P[s];
	return res;
}
 
template <int SIMPLIN_N>
void
SimplexLin<SIMPLIN_N>::printInfo() const {
	cout << " --- SimplexLin Information --- " << endl;
	cout << " Number of coordinates:         " << nsc << endl;
	cout << " Order:                         " << n << endl;
	cout << " Number of nodes:               " << np << endl;
	cout << " Number of face nodes:          " << nfp << endl;
	cout << " Order of cubature integration: " << nc << endl;
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
	cout << " List of node indices:          " << endl;
	for (unsigned int i = 0; i < np; i++) {
		nId[i].printInfo();
		cout << endl;
	}
	cout << " List of side nodes indices:    " << endl;
	sNId.printInfo();
	cout << " Lagrange polynomials:          " << endl;
	for (unsigned int i = 0; i < np; i++) {
		cout << "Lagrange polynomial of node #" <<  i << endl;
		lagr[i].printInfo();
	}
	cout << " Lagrange polynomials derivatives: " << endl;
	for (unsigned int i = 0; i < np; i++) {
		for (unsigned int j = 0; j < faces; j++) {
			cout << "Pol. " << i << " derived w.r.t. var." << j << endl;
			dLagr[i][j].printInfo();
		}
	}
	cout << " Cubature positions and weights: " << endl;
	for (unsigned int i = 0; i < ncp; i++) {
		cout << "#" << i << " ";
		cPos[i].printInfo();
		cout << " " << cw[i] << endl;
	}
	cout << " --- End of simplex information --- " << endl;
}
