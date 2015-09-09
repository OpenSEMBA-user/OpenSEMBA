
template<class T>
FunctionPolynomial<T>::FunctionPolynomial() {
	nv_ = 0;
	nm_ = 0;
}
template<class T>
FunctionPolynomial<T>::FunctionPolynomial(const UInt nvar) {
	nv_ = nvar;
	nm_ = 0;
}

template<class T>
inline FunctionPolynomial<T>::~FunctionPolynomial() {
}

template<class T>
FunctionPolynomial<T>& FunctionPolynomial<T>::operator=(const FunctionPolynomial<T> &param) {
	if (this == &param)
		return *this;
	nv_ = param.nv_;
	nm_ = param.nm_;
	mv_ = param.mv_;
	mp_ = param.mp_;
	return *this;
}
 
template<class T>
inline UInt FunctionPolynomial<T>::numberOfVariables() const {
	return nv_;
}

template<class T>
inline UInt FunctionPolynomial<T>::numberOfMonomials() const {
	return nm_;
}

template<class T>
inline T FunctionPolynomial<T>::monomialValue(const UInt i) const {
	return mv_[i];
}

template<class T>
inline UInt FunctionPolynomial<T>::monomialPower(
 const UInt monomial,
 const UInt variable) const {
	return mp_[monomial][variable];
}
 
template<class T>
T FunctionPolynomial<T>::eval(const CartesianVector<T,1>& pos) const {
	assert(1 == nv_);
	T prod;
	UInt m, v;
	T res = 0.0;
	for (m = 0; m < nm_; m++) {
		prod = 1.0;
		for (v = 0; v < nv_; v++)
			prod *= pow(pos(v), mp_[m][v]);
		res += mv_[m] * prod;
	}
	return res;
}

template<class T>
T FunctionPolynomial<T>::eval(const CartesianVector<T,2>& pos) const {
	assert(2 == nv_);
	T prod;
	UInt m, v;
	T res = 0.0;
	for (m = 0; m < nm_; m++) {
		prod = 1.0;
		for (v = 0; v < nv_; v++)
			prod *= pow(pos(v), mp_[m][v]);
		res += mv_[m] * prod;
	}
	return res;
}

template<class T>
T FunctionPolynomial<T>::eval(const CartesianVector<T,3>& pos) const {
	assert(3 == nv_);
	T prod;
	UInt m, v;
	T res = 0.0;
	for (m = 0; m < nm_; m++) {
		prod = 1.0;
		for (v = 0; v < nv_; v++)
			prod *= pow(pos(v), mp_[m][v]);
		res += mv_[m] * prod;
	}
	return res;
}

template<class T>
T FunctionPolynomial<T>::eval(const CartesianVector<T,4>& pos) const {
	assert(4 == nv_);
	T prod;
	UInt m, v;
	T res = 0.0;
	for (m = 0; m < nm_; m++) {
		prod = 1.0;
		for (v = 0; v < nv_; v++)
			prod *= pow(pos(v), mp_[m][v]);
		res += mv_[m] * prod;
	}
	return res;
}
 
template<class T>
void FunctionPolynomial<T>::derive(Int coord) {
	// Performs derivative with respect to coordinate coord.
	for (UInt m = 0; m < nm_; m++)
		if (mp_[m][coord] == 0)
			mv_[m] = 0.0;
		else {
			mv_[m] *= mp_[m][coord];
			mp_[m][coord]--;
		}
	// Removes monomials with value zero.
	removeZeros();
}
 
template<class T>
void FunctionPolynomial<T>::removeZeros() {
	for (UInt i = 0; i < nm_; i++)
		if (mv_[i] == 0) {
			nm_--;
			mv_.erase(mv_.begin() + i);
			mp_.erase(mp_.begin() + i);
		}
}
 
template<class T>
FunctionPolynomial<T> FunctionPolynomial<T>::operator*(const FunctionPolynomial<T> &param) const {
	// PURPOSE:
	// Performs polynomial product.
	// Algebraically, convolution is the same operation as multiplying
	assert(nv_ == 1 && param.nv_ == 1);
	DynMatrix<T> a, b, c;
	FunctionPolynomial<T> res(1);
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
FunctionPolynomial<T>& FunctionPolynomial<T>::operator*=(const FunctionPolynomial<T> &param) {
	assert(nv_ == 1 && param.nv_ == 1);
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
FunctionPolynomial<T> FunctionPolynomial<T>::operator^(const FunctionPolynomial<T> &param) {
	// PURPOSE: Performs external product between polynomials. This means that
	// variables in both polynomials will be treated as if they are different.
	UInt i, j;
	FunctionPolynomial<T> res(nv_ + param.nv_);
	vector<Int> pow;
	for (i = 0; i < nm_; i++)
		for (j = 0; j < param.nm_; j++) {
			pow = mp_[i];
			pow.insert(pow.end(), param.mp_[j].begin(), param.mp_[j].end());
			res.addMonomial(mv_[i]*param.mv_[j], pow);
		}
	return res;
}
 
template<class T>
FunctionPolynomial<T>& FunctionPolynomial<T>::operator^=(const FunctionPolynomial<T> &param) {
	// PURPOSE: Performs external product between polynomials. This means that
	// variables in both polynomials will be treated as if they are different.
	UInt i, j;
	FunctionPolynomial<T> res(nv_ + param.nv_);
	vector<Int> pow;
	for (i = 0; i < nm_; i++)
		for (j = 0; j < param.nm_; j++) {
			pow = mp_[i];
			pow.insert(pow.end(), param.mp_[j].begin(), param.mp_[j].end());
			res.addMonomial(mv_[i]*param.mv_[j], pow);
		}
	*this = res;
	return *this;
}
 
template<class T>
FunctionPolynomial<T>& FunctionPolynomial<T>::operator/=(const T param) {
	assert(param != 0);
	for (UInt i = 0 ; i < nm_; i++)
		mv_[i] /= param;
	return *this;
}
 
template<class T>
FunctionPolynomial<T> FunctionPolynomial<T>::vectorToPolynomial (T *v, Int sv, Int nvar) {
	assert(nvar == 1);
	FunctionPolynomial<T> res(1);
	for (UInt i = 0; i < sv ; i++)
		if (v[i] != 0)
			res.addMonomial(v[i], i);
	return res;
}
 
template<class T>
FunctionPolynomial<T> FunctionPolynomial<T>::matrixToPolynomial (const DynMatrix<T> &param) const {
	assert(nv_ <= 2);
	FunctionPolynomial<T> res;
	if (param.nCols() == 1) {
		res.nv_ = 1;
		for (UInt i = 0; i < param.nRows(); i++)
			if (param(i,0) != 0.0)
				res.addMonomial(param(i,0), i);
	} else {
		vector<Int> pow(2,0);
		res.nv_ = 2;
		for (UInt i = 0; i < param.nRows(); i++)
			for (UInt j = 0; j < param.nCols(); j++)
				if (param(i,j) != 0.0) {
					pow[0] = i;
					pow[1] = j;
					res.addMonomial(param(i,j), pow);
				}
	}
	return res;
}
 
template<class T>
DynMatrix<T> FunctionPolynomial<T>::polynomialToMatrix() const {
	assert(nv_ == 1);
	DynMatrix<T>  res(maxPower() + 1, 1);
	// Copies monomials to vector positions.
	for (UInt i = 0; i < nm_; i++)
		res(mp_[i][0], 0) = mv_[i];
	return res;
}
 
template<class T>
void FunctionPolynomial<T>::addMonomial(T val, vector<Int> pow) {
	assert(nv_ == pow.size());
	nm_++;
	mv_.push_back(val);
	mp_.push_back(pow);
}
 
template<class T>
void FunctionPolynomial<T>::addMonomial(T val, Int pow) {
	assert(nv_ == 1);
	vector<Int> vPow(1, pow);
	nm_++;
	mv_.push_back(val);
	mp_.push_back(vPow);
}
 
template<class T>
Int FunctionPolynomial<T>::maxPower() const {
	// Returns maximum power present in this polynomial.
	UInt i, j;
	Int res = 0;
	for (i = 0; i < nv_; i++)
		for (j = 0; j < nm_; j++)
			if (mp_[j][i] > res)
				res = mp_[j][i];
	return res;
}

template<class T>
inline T FunctionPolynomial<T>::operator ()(const T& arg) const {
    return eval(CartesianVector<T,1>(arg));
}

template<class T>
void FunctionPolynomial<T>::printInfo() const {
	cout << " -- Polynomial<T> -- " << endl;
	cout << "Number of variables:			" << nv_ << endl;
	cout << "Number of monomials:			" << nm_ << endl;
	cout << "Value		Powers		" << endl;
	UInt i, j;
	for (i = 0; i < nm_; i++) {
		cout << mv_[i] << "               ";
		for (j = 0; j < nv_; j++)
			cout << mp_[i][j] << " ";
		cout << endl;
	}
}
 
