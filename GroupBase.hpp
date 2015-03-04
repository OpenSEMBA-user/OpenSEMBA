//#ifndef GROUPBASE_H_
//#include "GroupBase.h"
//#endif

template<typename T>
GroupBase<T>::GroupBase() {

}

template<typename T>
GroupBase<T>::GroupBase(const vector<T*>& elems) {
    element_.resize(elems.size());
    for(unsigned i = 0; i < elems.size(); i++) {
        this->element_[i] = elems[i]->clone()->castTo<T>();
    }
}

template<typename T>
GroupBase<T>::GroupBase(const GroupBase<T>& rhs) {
    element_.resize(rhs.element_.size());
    for(unsigned i = 0; i < rhs.element_.size(); i++)
        element_[i] = rhs.element_[i]->clone()->castTo<T>();
}

template<typename T>
GroupBase<T>::~GroupBase() {
    for(unsigned i = 0; i < element_.size(); i++)
        delete element_[i];
}

template<typename T>
GroupBase<T>& GroupBase<T>::operator=(const GroupBase<T>& rhs) {
    if(this == &rhs)
        return *this;

    for(unsigned i = 0; i < element_.size(); i++)
        delete element_[i];

    element_.resize(rhs.element_.size());
    for(unsigned i = 0; i < rhs.element_.size(); i++)
        element_[i] = rhs.element_[i]->clone()->castTo<T>();

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
        if(this->element_[i]->isOf<T2>())
            elems.push_back(this->element_[i]->castTo<T2>());

    return GroupBase<T2>(elems);
}

template<typename T>
void GroupBase<T>::add(const T* newElem) {
    this->element_.reserve(this->element_.size() + 1);
    this->element_.push_back(newElem->clone()->castTo<T>());
}

template<typename T>
void GroupBase<T>::add(const vector<T*>& newElems) {
    this->element_.reserve(this->element_.size() + newElems.size());
    for (unsigned i = 0; i < newElems.size(); i++) {
        this->element_.push_back(newElems[i]->clone()->castTo<T>());
    }
}
