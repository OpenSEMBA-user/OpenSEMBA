// =============== Includes headers ===========================================
#include "Polynomial.h"
// ===============Polynomial methods ==========================================
template<class T>
Polynomial<T>::Polynomial() {
	nv = 0;
	nm = 0;
}
 
template<class T>
Polynomial<T>::Polynomial(const unsigned int nvar) {
	nv = nvar;
	nm = 0;
}
// =-=-=-=-=-=-=-= Destructors =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// =-=-=-=-=-=-=-= General methods =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template<class T>
Polynomial<T>&
Polynomial<T>::operator=(const Polynomial<T> &param) {
	if (this == &param)
		return *this;
	nv = param.nv;
	nm = param.nm;
	mv = param.mv;
	mp = param.mp;
	return *this;
}
 
template<class T>
inline unsigned int
Polynomial<T>::numberOfVariables() const {
	return nv;
}

template<class T>
inline unsigned int
Polynomial<T>::numberOfMonomials() const {
	return nm;
}

template<class T>
inline T
Polynomial<T>::monomialValue(const unsigned int i) const {
	return mv[i];
}

template<class T>
inline unsigned int
Polynomial<T>::monomialPower(
 const unsigned int monomial,
 const unsigned int variable) const {
	return mp[monomial][variable];
}
 
template<class T>
T
Polynomial<T>::eval(const CartesianVector<T,1>& pos) const {
	assert(1 == nv);
	T prod;
	unsigned int m, v;
	T res = 0.0;
	for (m = 0; m < nm; m++) {
		prod = 1.0;
		for (v = 0; v < nv; v++)
			prod *= pow(pos(v), mp[m][v]);
		res += mv[m] * prod;
	}
	return res;
}

template<class T>
T
Polynomial<T>::eval(const CartesianVector<T,2>& pos) const {
	assert(2 == nv);
	T prod;
	unsigned int m, v;
	T res = 0.0;
	for (m = 0; m < nm; m++) {
		prod = 1.0;
		for (v = 0; v < nv; v++)
			prod *= pow(pos(v), mp[m][v]);
		res += mv[m] * prod;
	}
	return res;
}

template<class T>
T
Polynomial<T>::eval(const CartesianVector<T,3>& pos) const {
	assert(3 == nv);
	T prod;
	unsigned int m, v;
	T res = 0.0;
	for (m = 0; m < nm; m++) {
		prod = 1.0;
		for (v = 0; v < nv; v++)
			prod *= pow(pos(v), mp[m][v]);
		res += mv[m] * prod;
	}
	return res;
}

template<class T>
T
Polynomial<T>::eval(const CartesianVector<T,4>& pos) const {
	assert(4 == nv);
	T prod;
	unsigned int m, v;
	T res = 0.0;
	for (m = 0; m < nm; m++) {
		prod = 1.0;
		for (v = 0; v < nv; v++)
			prod *= pow(pos(v), mp[m][v]);
		res += mv[m] * prod;
	}
	return res;
}
 
template<class T>
void
Polynomial<T>::derive(int coord) {
	// Performs derivative with respect to coordinate coord.
	for (unsigned int m = 0; m < nm; m++)
		if (mp[m][coord] == 0)
			mv[m] = 0.0;
		else {
			mv[m] *= mp[m][coord];
			mp[m][coord]--;
		}
	// Removes monomials with value zero.
	removeZeros();
}
 
template<class T>
void
Polynomial<T>::removeZeros() {
	for (unsigned i = 0; i < nm; i++)
		if (mv[i] == 0) {
			nm--;
			mv.erase(mv.begin() + i);
			mp.erase(mp.begin() + i);
		}
}
 
template<class T>
Polynomial<T>
Polynomial<T>::operator*(const Polynomial<T> &param) const {
	// PURPOSE:
	// Performs polynomial product.
	// Algebraically, convolution is the same operation as multiplying
	assert(nv == 1 && param.nv == 1);
	DynMatrix<T> a, b, c;
	Polynomial<T> res(1);
	// Converts polynomials to vectors.
	a = polynomialToMatrix();
	b = param.polynomialToMatrix();
	// Performs product as vector convolution.
	c = a.convolute(b);
	// Copies result in this polynomial.
	res = matrixToPolynomial(c);
	// Returns result.
	return res;
}
 
template<class T>
Polynomial<T>&
Polynomial<T>::operator*=(const Polynomial<T> &param) {
	assert(nv == 1 && param.nv == 1);
	DynMatrix<T> a, b, c;
	// Converts polynomials to vectors.
	a = polynomialToMatrix();
	b = param.polynomialToMatrix();
	// Performs product as vector convolution.
	c = a.convolute(b);
	// Copies ;result in this polynomial.
	*this = matrixToPolynomial(c);
	// Returns result.
	return *this;
}
 
template<class T>
Polynomial<T>
Polynomial<T>::operator^(const Polynomial<T> &param) {
	// PURPOSE:
	// Performs external product between polynomials. This means that
	// variables in both polynomials will be treated as if they are different.
	unsigned int i, j;
	Polynomial<T> res(nv + param.nv);
	vector<int> pow;
	for (i = 0; i < nm; i++)
		for (j = 0; j < param.nm; j++) {
			pow = mp[i];
			pow.insert(pow.end(), param.mp[j].begin(), param.mp[j].end());
			res.addMonomial(mv[i]*param.mv[j], pow);
		}
	return res;
}
 
template<class T>
Polynomial<T>&
Polynomial<T>::operator^=(const Polynomial<T> &param) {
	// PURPOSE: Performs external product between polynomials. This means that
	// variables in both polynomials will be treated as if they are different.
	unsigned int i, j;
	Polynomial<T> res(nv + param.nv);
	vector<int> pow;
	for (i = 0; i < nm; i++)
		for (j = 0; j < param.nm; j++) {
			pow = mp[i];
			pow.insert(pow.end(), param.mp[j].begin(), param.mp[j].end());
			res.addMonomial(mv[i]*param.mv[j], pow);
		}
	*this = res;
	return *this;
}
 
template<class T>
Polynomial<T>&
Polynomial<T>::operator/=(const T param) {
	assert(param != 0);
	for (unsigned int i = 0 ; i < nm; i++)
		mv[i] /= param;
	return *this;
}
 
template<class T>
Polynomial<T>
Polynomial<T>::vectorToPolynomial (T *v, int sv, int nvar) {
	assert(nvar == 1);
	Polynomial<T> res(1);
	for (unsigned int i = 0; i < sv ; i++)
		if (v[i] != 0)
			res.addMonomial(v[i], i);
	return res;
}
 
template<class T>
Polynomial<T>
Polynomial<T>::matrixToPolynomial (const DynMatrix<T> &param) const {
	assert(nv <= 2);
	Polynomial<T> res;
	if (param.nCols() == 1) {
		res.nv = 1;
		for (unsigned int i = 0; i < param.nRows(); i++)
			if (param(i,0) != 0.0)
				res.addMonomial(param(i,0), i);
	} else {
		vector<int> pow(2,0);
		res.nv = 2;
		for (unsigned int i = 0; i < param.nRows(); i++)
			for (unsigned int j = 0; j < param.nCols(); j++)
				if (param(i,j) != 0.0) {
					pow[0] = i;
					pow[1] = j;
					res.addMonomial(param(i,j), pow);
				}
	}
	return res;
}
 
template<class T>
DynMatrix<T>
Polynomial<T>::polynomialToMatrix() const {
	assert(nv == 1);
	DynMatrix<T>  res(maxPower() + 1, 1);
	// Copies monomials to vector positions.
	for (unsigned int i = 0; i < nm; i++)
		res(mp[i][0], 0) = mv[i];
	return res;
}
 
template<class T>
void
Polynomial<T>::addMonomial(T val, vector<int> pow) {
	assert(nv == pow.size());
	nm++;
	mv.push_back(val);
	mp.push_back(pow);
}
 
template<class T>
void
Polynomial<T>::addMonomial(T val, int pow) {
	assert(nv == 1);
	vector<int> vPow(1, pow);
	nm++;
	mv.push_back(val);
	mp.push_back(vPow);
}
 
template<class T>
int
Polynomial<T>::maxPower() const {
	// Returns maximum power present in this polynomial.
	unsigned int i, j;
	int res = 0;
	for (i = 0; i < nv; i++)
		for (j = 0; j < nm; j++)
			if (mp[j][i] > res)
				res = mp[j][i];
	return res;
}
 
template<class T>
void
Polynomial<T>::printInfo() const {
	cout << " -- Polynomial<T> -- " << endl;
	cout << "Number of variables:			" << nv << endl;
	cout << "Number of monomials:			" << nm << endl;
	cout << "Value		Powers		" << endl;
	unsigned int i, j;
	for (i = 0; i < nm; i++) {
		cout << mv[i] << "               ";
		for (j = 0; j < nv; j++)
			cout << mp[i][j] << " ";
		cout << endl;
	}
}
 
