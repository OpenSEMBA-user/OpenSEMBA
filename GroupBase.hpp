#ifndef GROUPBASE_H_
#include "GroupBase.h"
#endif

template<typename T, class Id>
GroupBase<T, Id>::GroupBase() {

}

template<typename T, class Id>
GroupBase<T, Id>::GroupBase(const vector<T*>& elems) {
    element_.resize(elems.size());
    for(unsigned i = 0; i < elems.size(); i++) {
        this->element_[i] = elems[i]->clone()->template castTo<T>();
    }
}

template<typename T, class Id>
GroupBase<T, Id>::GroupBase(const GroupBase<T, Id>& rhs) {
    element_.resize(rhs.element_.size());
    for(unsigned i = 0; i < rhs.element_.size(); i++)
        element_[i] = rhs.element_[i]->clone()->template castTo<T>();
}

template<typename T, class Id>
GroupBase<T, Id>::~GroupBase() {
    for(unsigned i = 0; i < element_.size(); i++)
        delete element_[i];
}

template<typename T, class Id>
GroupBase<T>& GroupBase<T, Id>::operator=(const GroupBase<T, Id>& rhs) {
    if(this == &rhs)
        return *this;

    for(unsigned i = 0; i < element_.size(); i++)
        delete element_[i];

    element_.resize(rhs.element_.size());
    for(unsigned i = 0; i < rhs.element_.size(); i++)
        element_[i] = rhs.element_[i]->clone()->template castTo<T>();

    return *this;
}

template<typename T, class Id>
const T* GroupBase<T, Id>::operator()(const unsigned i) const {
    return element_[i];
}

template<typename T, class Id> template<typename T2>
GroupBase<T2> GroupBase<T, Id>::getGroupOf() const {
    vector<T2*> elems;
    for (unsigned i = 0; i < this->size(); i++)
        if(this->element_[i]->template isOf<T2>())
            elems.push_back(this->element_[i]->template castTo<T2>());

    return GroupBase<T2, Id>(elems);
}

template<typename T, class Id>
Id GroupBase<T, Id>::add(const T* newElem) {
    vector<T*> aux;
    T* newElem_ = newElem->clone()->template castTo<T>();
    aux.push_back(newElem_);
    add(aux);
    delete newElem_;
    return 0;
}

template<typename T, class Id>
vector<Id> GroupBase<T, Id>::add(const vector<T*>& newElems) {
    this->element_.reserve(this->element_.size() + newElems.size());
    for (unsigned i = 0; i < newElems.size(); i++) {
        this->element_.push_back(
            newElems[i]->clone()->template castTo<T>());
    }
    
    return vector<Id>();
}
