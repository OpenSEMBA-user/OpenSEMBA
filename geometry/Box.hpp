/*
 * Box.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: luis
 */

template<class T, int D>
Box<T,D>::Box() {
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
    minMax_ = pair<CVecTD, CVecTD>(min, max);
}

template<class T, int D>
void Box<T,D>::set(const pair<CVecTD, CVecTD>& minMax) {
    minMax_ = minMax;
}

template<class T, int D>
bool
Box<T,D>::operator > (const Box<T,D> &rhs)const{
    for (int i = 0; i < D; i++) {
        if (minMax_.second(i) <= rhs.minMax_.second(i)) {
            return false;
        }
        if (minMax_.first(i) >= rhs.minMax_.first(i)) {
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
Box<T,D>::operator == (const Box<T,D> &rhs) const{
    if (minMax_.second != rhs.minMax_.second) {return false;}
    if (minMax_.first != rhs.minMax_.first) {return false;}
    return true;
}

template<class T, int D>
void
Box<T,D>::operator += (const Box<T,D> &rhs){
    for (int i = 0; i < D; i++) {
        if (minMax_.second(i) < rhs.minMax_.second(i)) {
            minMax_.second(i) = rhs.minMax_.second(i);
        }
        if (minMax_.first(i) > rhs.minMax_.first(i)) {
            minMax_.first(i) = rhs.minMax_.first(i);
        }
    }
}

template<class T, int D>
void
Box<T,D>::operator = (const Box<T,D> &rhs){
    minMax_ = rhs.minMax_;
}

template<class T, int D>
void
Box<T,D>::operator << (const CVecTD &p){
    if(minMax_.second(0)<p(0)){ minMax_.second(0)=p(0); }
    if(minMax_.second(1)<p(1)){ minMax_.second(1)=p(1); }
    if(minMax_.second(2)<p(2)){ minMax_.second(2)=p(2); }

    if(minMax_.first(0)>p(0)){ minMax_.first(0)=p(0); }
    if(minMax_.first(1)>p(1)){ minMax_.first(1)=p(1); }
    if(minMax_.first(2)>p(2)){ minMax_.first(2)=p(2); }
}

template<class T, int D>
bool
Box<T,D>::isIntersected(const Box<T,D> &rhs) const {
    for (int i = 0; i < D; i++) {
        if (minMax_.second(i) < rhs.minMax_.first(i)) {
            return false;
        }
        if (rhs.minMax_.second(i) < minMax_.first(i)) {
            return false;
        }
    }
    return true;
}

template<class T, int D>
void
Box<T,D>::printInfo() const {
    cout<< "Box info" << endl;
    cout<< "Min: ";
    minMax_.first.printInfo();
    cout<< ", Max: ";
    minMax_.second.printInfo();
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
    minMax_.first *= factor;
    minMax_.second *= factor;
}
