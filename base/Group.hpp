#ifndef GROUP_H_
#include "Group.h"
#endif

template<typename T>
Group<T>::Group()
:   ownership_(true) {

}

template<typename T> template<typename T2>
Group<T>::Group(const vector<T2*>& elems)
:   ownership_(true) {

    initElements(elems);
}

template<typename T> template<typename T2>
Group<T>::Group(const vector<T2*>& elems, bool ownership)
:   ownership_(ownership) {
    if (ownership_) {
        cloneElements(elems);
    } else {
        initElements(elems);
    }
}

template<typename T>
Group<T>::Group(const Group<T>& rhs)
:   ownership_(rhs.ownership_) {
    if(rhs.ownership_) {
        cloneElements(rhs.element_);
    } else {
        initElements(rhs.element_);
    }
}

template<typename T> template<typename T2>
Group<T>::Group(const Group<T2>& rhs)
:   ownership_(rhs.ownership_) {
    if(rhs.ownership_) {
        cloneElements(rhs.element_);
    } else {
        initElements(rhs.element_);
    }
}

template<typename T>
Group<T>::~Group() {
    clear();
}

template<typename T>
ClassBase* Group<T>::clone() const {
    return new Group<T>(*this);
}

template<typename T>
Group<T>& Group<T>::operator=(const Group<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    clear();
    this->ownership_ = rhs.ownership_;
    if(rhs.ownership_) {
        cloneElements(rhs.element_);
    } else {
        initElements(rhs.element_);
    }
    return *this;
}

template<typename T> template<typename T2>
Group<T>& Group<T>::operator=(const Group<T2>& rhs) {
    clear();
    this->ownership_ = rhs.ownership_;
    if(rhs.ownership_) {
        cloneElements(rhs.element_);
    } else {
        initElements(rhs.element_);
    }
    return *this;
}

template<typename T>
void Group<T>::clear() {
    if(this->ownership_) {
        for(UInt i = 0; i < element_.size(); i++) {
            delete element_[i];
        }
    }
    element_.clear();
}

template<typename T> template<typename T2>
UInt Group<T>::sizeOf() const {
    UInt res = 0;
    for (UInt i = 0; i < this->size(); i++) {
        if(this->element_[i]->template is<T2>()) {
            res++;
        }
    }
    return res;
}

//template<typename T>
//T* Group<T>::operator()(const UInt i) {
//    return element_[i];
//}

template<typename T>
const T* Group<T>::operator()(const UInt i) const {
    return element_[i];
}

template<typename T> template<typename T2>
Group<T2> Group<T>::newGroupOf() const {
    vector<T2*> elems;
    for (UInt i = 0; i < this->size(); i++) {
        if(this->element_[i]->template is<T2>()) {
            elems.push_back(this->element_[i]->template castTo<T2>());
        }
    }
    return Group<T2>(elems, true);
}

template<typename T> template<typename T2>
Group<T2> Group<T>::getGroupOf() const {
    vector<T2*> elems;
    for (UInt i = 0; i < this->size(); i++) {
        if(this->element_[i]->template is<T2>()) {
            elems.push_back(this->element_[i]->template castTo<T2>());
        }
    }
    return Group<T2>(elems, false);
}

//template<typename T> template<typename T2>
//vector<const T2*> Group<T>::getVectorOf() const {
//    vector<const T2*> elems;
//    for (UInt i = 0; i < this->size(); i++) {
//        if(this->element_[i]->template is<T2>()) {
//            elems.push_back(this->element_[i]->template castTo<T2>());
//        }
//    }
//    return elems;
//}

template<typename T>
void Group<T>::reserve(const UInt nS) {
    this->element_.reserve(nS);
}

template<typename T> template<typename T2>
void Group<T>::add(T2* newElem) {
    vector<T2*> aux;
    aux.push_back(newElem);
    add(aux);
}

template<typename T> template<typename T2>
void Group<T>::add(vector<T2*>& newElems) {
    if(!this->ownership_) {
        cerr << endl << "ERROR @ Group::add(): "
             << "Forbidden to add elements to a Group without ownership "
             << "of elements on it" << endl;
        assert(false);
        exit(EXIT_FAILURE);
    }

    for (UInt i = 0; i < newElems.size(); i++) {
        if (newElems[i]->template is<T>()) {
            this->element_.push_back(newElems[i]->template castTo<T>());
        }
    }
}

template<typename T> template<typename T2>
void Group<T>::add(const Group<T2>& rhs) {
    if(!this->ownership_) {
        cerr << endl << "ERROR @ Group::add(): "
             << "Forbidden to add elements to a Group without ownership "
             << "of elements on it" << endl;
        assert(false);
        exit(EXIT_FAILURE);
    }

    for (UInt i = 0; i < rhs.size(); i++) {
        if (rhs(i)->template is<T>()) {
            this->element_.push_back(rhs(i)->clone()->template castTo<T>());
        }
    }
}

template<typename T> template<typename T2>
void Group<T>::initElements(const vector<T2*>& elems) {
    this->element_.reserve(elems.size());
    for(UInt i = 0; i < elems.size(); i++) {
        if (elems[i]->template is<T>()) {
            this->element_.push_back(elems[i]->template castTo<T>());
        }
    }
}

template<typename T> template<typename T2>
void Group<T>::cloneElements(const vector<T2*>& elems) {
    this->ownership_ = true;
    this->element_.reserve(elems.size());
    for(UInt i = 0; i < elems.size(); i++) {
        ClassBase* newElem = elems[i]->clone();
        if (newElem->template is<T>()) {
            this->element_.push_back(newElem->template castTo<T>());
        } else {
            delete newElem;
        }
    }
}

template<typename T>
void Group<T>::printInfo() const {
    cout<< "--- Group info ---" << endl;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
        cout << endl;
    }
}
