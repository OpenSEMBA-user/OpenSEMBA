#ifndef GROUPBASE_H_
#include "GroupBase.h"
#endif

template<typename T>
GroupBase<T>::GroupBase() {

}

template<typename T>
GroupBase<T>::GroupBase(const vector<T*>& elems) {
    cloneElements(elems);
}

template<typename T>
GroupBase<T>::GroupBase(const GroupBase<T>& rhs) {
    cloneElements(rhs.element_);
}

template<typename T>
GroupBase<T>::~GroupBase() {
    deleteElements();
}

template<typename T>
GroupBase<T>& GroupBase<T>::operator=(const GroupBase<T>& rhs) {
    if(this == &rhs)
        return *this;

    deleteElements();
    cloneElements(rhs.element_);

    return *this;
}

template<typename T>
const T* GroupBase<T>::operator()(const unsigned i) const {
    return element_[i];
}

template<typename T> template<typename T2>
GroupBase<T2> GroupBase<T>::getGroupOf() const {
    vector<T2*> elems;
    for (unsigned i = 0; i < this->size(); i++)
        if(this->element_[i]->template isOf<T2>())
            elems.push_back(this->element_[i]->template castTo<T2>());

    return GroupBase<T2>(elems);
}

template<typename T> template<typename T2>
vector<const T2*> GroupBase<T>::getVectorOf() const {
    vector<const T2*> elems;
    for (unsigned i = 0; i < this->size(); i++)
        if(this->element_[i]->template isOf<T2>())
            elems.push_back(this->element_[i]->template castTo<T2>());

    return elems;
}

template<typename T> template<typename T2>
unsigned GroupBase<T>::numberOf() const {
    unsigned res = 0;
    for (unsigned i = 0; i < this->size(); i++)
        if(this->element_[i]->template isOf<T2>())
            res++;

    return res;
}

template<typename T>
const T* GroupBase<T>::add(T* newElem) {
    vector<const T*> res;
    vector<T*> aux;
    aux.push_back(newElem);
    res = add(aux);

    assert(res.size() == 1);
    return res[0];
}

template<typename T>
vector<const T*> GroupBase<T>::add(const vector<T*>& newElems) {
    vector<const T*> res;
    this->element_.reserve(this->element_.size() + newElems.size());
    for (unsigned i = 0; i < newElems.size(); i++) {
        this->element_.push_back(
            newElems[i]->clone()->template castTo<T>());
        res.push_back(this->element_.back());
    }
    assert(res.size() == newElems.size());
    return res;
}

template<typename T>
void GroupBase<T>::cloneElements(const vector<T*>& elems) {
    this->element_.resize(elems.size());
    for(unsigned i = 0; i < elems.size(); i++) {
        this->element_[i] = elems[i]->clone()->template castTo<T>();
    }
}

template<typename T>
void GroupBase<T>::deleteElements() {
    for(unsigned i = 0; i < element_.size(); i++)
        delete element_[i];
}
