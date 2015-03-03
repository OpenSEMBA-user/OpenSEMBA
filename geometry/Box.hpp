/*
 * Box.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: luis
 */

template<class T, int D>
Box<T,D>::Box() {
   setDefaultValues();
}

template<class T, int D>
Box<T,D>::~Box() {
}

template<class T, int D> Box<T,D>::Box(
      const pair<CVecTD, CVecTD>& bounds) {
   set(bounds);
}

template<class T, int D>
Box<T,D>::Box(const CVecTD& min, const CVecTD& max) {
   min_ = min;
   max_ = max;
}

template<class T, int D>
void Box<T,D>::set(const pair<CVecTD, CVecTD>& minMax) {
   min_ = minMax.first;
   max_ = minMax.second;
}

template<class T, int D>
bool
Box<T,D>::operator > (const Box<T,D> &rhs)const{
   for (int i = 0; i < D; i++) {
      if ( (max_(i) <= rhs.max_(i)) || (min_(i) >= rhs.min_(i)) ) {
         return false;
      }
   }
   return true;
}

template<class T, int D>
bool
Box<T,D>::operator < (const Box<T,D> &lBoxMax) const{
   return lBoxMax > *this;
}

template<class T, int D>
bool
Box<T,D>::operator <= (const Box<T,D> &rhs) const{
   for (int i = 0; i < D; i++) {
      if ( (max_(i) > rhs.max_(i)) || (min_(i) < rhs.min_(i)) ) {
         return false;
      }
   }
   return true;
}

template<class T, int D>
bool
Box<T,D>::operator == (const Box<T,D> &rhs) const{
   if (max_ != rhs.max_) {return false;}
   if (min_ != rhs.min_) {return false;}
   return true;
}

template<class T, int D>
void
Box<T,D>::operator += (const Box<T,D> &rhs){
   for (int i = 0; i < D; i++) {
      if (max_(i) < rhs.max_(i)) {
         max_(i) = rhs.max_(i);
      }
      if (min_(i) > rhs.min_(i)) {
         min_(i) = rhs.min_(i);
      }
   }
}

template<class T, int D>
Box<T,D>&
Box<T,D>::operator = (const Box<T,D> &rhs){
   if (&rhs == this) {
      return *this;
   }
   min_ = rhs.min_;
   max_ = rhs.max_;
   return *this;
}

template<class T, int D>
void
Box<T,D>::operator << (const CVecTD& p){
   for (int i = 0; i < D; i++) {
      if (min_(i) > p(i)) {
         min_(i) = p(i);
      }
      if (max_(i) < p(i)) {
         max_(i) = p(i);
      }
   }
}

template<class T, int D>
void
Box<T,D>::operator << (const Box<T,D>& p){
   *this << p.min_;
   *this << p.max_;
}

template<class T, int D>
bool
Box<T,D>::isIntersected(const Box<T,D> &rhs) const {
   for (int i = 0; i < D; i++) {
      if (max_(i) < rhs.min_(i)) {
         return false;
      }
      if (rhs.max_(i) < min_(i)) {
         return false;
      }
   }
   return true;
}

template<class T, int D>
inline Box<T,D>& Box<T,D>::setInfinity() {
   for (int j = 0; j < D; j++) {
      min_(j) = - numeric_limits<T>::infinity();
      max_(j) = numeric_limits<T>::infinity();
   }
   return *this;
}

template<class T, int D>
inline CartesianVector<T,D> Box<T,D>::getMin(void) const {
   return min_;
}

template<class T, int D>
inline CartesianVector<T,D> Box<T,D>::getMax(void) const {
   return max_;
}

template<class T, int D>
inline CartesianVector<T,D> Box<T,D>::getLength() const {
   return (max_ - min_);
}

template<class T, int D>
inline Box<T,D>
Box<T,D>::intersect(const Box<T, D>& rhs) const {
   Box<T,D> res;
   for (int d = 0; d < D; d++) {
      if (min_(d) > rhs.min_(d)) {
         res.min_(d) = min_(d);
      } else {
         res.min_(d) = rhs.min_(d);
      }
      if (max_(d) < rhs.max_(d)) {
         res.max_(d) = max_(d);
      } else {
         res.max_(d) = rhs.max_(d);
      }
   }
   return res;
}

template<class T, int D>
inline vector<CartesianVector<T, D> >
Box<T,D>::getPosOfBound(
        CartesianAxis d,
        CartesianBound p) const {
    assert(D == 3);
    int rX = d;
    int rY = (d+1)%D;
    int rZ = (d+2)%D;
    vector<CVecTD> res;
    // TODO: Generalize this...
    {
        CVecTD aux;
        aux(rX) = getBound(CartesianBound(p))(rX);
        aux(rY) = getBound(L)(rY);
        aux(rZ) = getBound(L)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(CartesianBound(p))(rX);
        aux(rY) = getBound(L)(rY);
        aux(rZ) = getBound(U)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(CartesianBound(p))(rX);
        aux(rY) = getBound(U)(rY);
        aux(rZ) = getBound(U)(rZ);
        res.push_back(aux);
    }
    {
        CVecTD aux;
        aux(rX) = getBound(CartesianBound(p))(rX);
        aux(rY) = getBound(U)(rY);
        aux(rZ) = getBound(L)(rZ);
        res.push_back(aux);
    }
    return res;
}

template<class T, int D>
inline CartesianVector<T,D>
Box<T,D>::getBound(CartesianBound p) const {
    if (p == L) {
        return getMin();
    } else {
        return getMax();
    }
}

template<class T, int D>
inline vector<CartesianVector<T,D> >
Box<T,D>::getPos() const {
    assert(D == 3);
    vector<CVecTD> res(8);
    res[0] = CVecTD(min_(x), min_(y), min_(z));
    res[1] = CVecTD(max_(x), min_(y), min_(z));
    res[2] = CVecTD(max_(x), max_(y), min_(z));
    res[3] = CVecTD(min_(x), max_(y), min_(z));
    res[4] = CVecTD(min_(x), min_(y), max_(z));
    res[5] = CVecTD(max_(x), min_(y), max_(z));
    res[6] = CVecTD(max_(x), max_(y), max_(z));
    res[7] = CVecTD(min_(x), max_(y), max_(z));
    return res;
}

template<class T, int D>
void
Box<T,D>::printInfo() const {
   cout<< "Box info" << endl;
   cout<< "Min: " << min_.toStr() << endl;
   cout<< "Max: " << max_.toStr() << endl;
   cout<< endl;
}

template<class T, int D>
bool
Box<T,D>::isInnerPoint(const CVecTD& point) const {
   bool isInner = true;
   for (int dir = 0; dir < D; dir++) {
      isInner &= (point(dir) <= getMax()(dir));
      isInner &= (point(dir) >= getMin()(dir));
   }
   return isInner;
}

template<class T, int D>
void
Box<T,D>::scale(const double factor) {
   min_ *= factor;
   max_ *= factor;
}
