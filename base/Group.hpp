#ifndef GROUP_H_
#include "Group.h"
#endif

template<typename T>
Group<T>::Group()
:   ownership_(true) {

}

template<typename T>
Group<T>::Group(const vector<T*>& elems, bool ownership)
:   ownership_(ownership) {

    initElements(elems);
}

template<typename T>
Group<T>::Group(const Group<T>& rhs)
:   ownership_(false) {

    if(rhs.ownership_)
        cloneElements(rhs.element_);
    else
        initElements(rhs.element_);
}

template<typename T>
Group<T>::~Group() {
    deleteElements();
}

template<typename T>
Group<T>& Group<T>::operator=(const Group<T>& rhs) {
    if(this == &rhs)
        return *this;

    deleteElements();
    this->ownership_ = false;
    if(rhs.ownership_)
        cloneElements(rhs.element_);
    else
        initElements(rhs.element_);

    return *this;
}

template<typename T>
T* Group<T>::operator()(const unsigned i) {
    return element_[i];
}

template<typename T>
const T* Group<T>::operator()(const unsigned i) const {
    return element_[i];
}

template<typename T> template<typename T2>
Group<T2> Group<T>::newGroupOf() const {
    vector<T2*> elems;
    for (unsigned i = 0; i < this->size(); i++) {
        if(this->element_[i]->template is<T2>()) {
            elems.push_back(this->element_[i]->clone()->template castTo<T2>());
        }
    }
    return Group<T2>(elems);
}

template<typename T> template<typename T2>
Group<T2> Group<T>::getGroupOf() const {
    vector<T2*> elems;
    for (unsigned i = 0; i < this->size(); i++) {
        if(this->element_[i]->template is<T2>()) {
            elems.push_back(this->element_[i]->template castTo<T2>());
        }
    }
    return Group<T2>(elems, false);
}

template<typename T> template<typename T2>
vector<const T2*> Group<T>::getVectorOf() const {
    vector<const T2*> elems;
    for (unsigned i = 0; i < this->size(); i++) {
        if(this->element_[i]->template is<T2>()) {
            elems.push_back(this->element_[i]->template castTo<T2>());
        }
    }
    return elems;
}

template<typename T> template<typename T2>
unsigned Group<T>::sizeOf() const {
    unsigned res = 0;
    for (unsigned i = 0; i < this->size(); i++) {
        if(this->element_[i]->template is<T2>()) {
            res++;
        }
    }
    return res;
}

template<typename T>
void Group<T>::add(T* newElem) {
    vector<T*> aux;
    aux.push_back(newElem);
    add(aux);
}

template<typename T>
void Group<T>::add(vector<T*>& newElems) {
    if(!this->ownership_) {
        cerr << "ERROR @ Group::add(): "
             << "Forbidden to add elements to a Group without ownership "
             << "of elements on it" << endl;
        assert(false);
        exit(1);
    }

    this->element_.reserve(this->element_.size() + newElems.size());
    for (unsigned i = 0; i < newElems.size(); i++) {
        this->element_.push_back(newElems[i]->template castTo<T>());
    }
}

template<typename T>
void Group<T>::initElements(const vector<T*>& elems) {
    this->element_.resize(elems.size());
    for(unsigned i = 0; i < elems.size(); i++) {
        this->element_[i] = elems[i];
    }
}

template<typename T>
void Group<T>::cloneElements(const vector<T*>& elems) {
    this->ownership_ = true;
    this->element_.resize(elems.size());
    for(unsigned i = 0; i < elems.size(); i++) {
        this->element_[i] = elems[i]->clone()->template castTo<T>();
    }
}

template<typename T>
void Group<T>::deleteElements() {
    if(this->ownership_) {
        for(unsigned i = 0; i < element_.size(); i++) {
            delete element_[i];
        }
    }
}
