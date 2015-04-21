#include "Group.h"

template<typename T>
Group<T>::Group() {
}

template<typename T> template<typename T2>
Group<T>::Group(T2* elems) {
    static_assert(is_const<T2>::value ? is_const<T>::value : true,
                  "Template parameter must be const");
    Group<T>::add(elems);
}

template<typename T> template<typename T2>
Group<T>::Group(const vector<T2*>& elems) {
    static_assert(is_const<T2>::value ? is_const<T>::value : true,
                  "Template parameter must be const");
    Group<T>::add(elems);
}

template<typename T>
Group<T>::Group(Group<T>& rhs) {
    Group<T>::add(rhs);
}

template<typename T> template<typename T2>
Group<T>::Group(Group<T2>& rhs) {
    static_assert(is_const<T2>::value ? is_const<T>::value : true,
                  "Template parameter must be const");
    Group<T>::add(rhs);
}

template<typename T> template<typename T2>
Group<T>::Group(const Group<T2>& rhs) {
    static_assert(is_const<T>::value, "Template parameter must be const");
    Group<T>::add(rhs);
}

template<typename T>
Group<T>::Group(Group<T>&& rhs) {
    Group<T>::add(std::move(rhs));
}

template<typename T> template<typename T2>
Group<T>::Group(Group<T2>&& rhs) {
    static_assert(is_const<T2>::value ? is_const<T>::value : true,
                  "Template parameter must be const");
    Group<T>::add(std::move(rhs));
}

template<typename T>
Group<T>::~Group() {
    clear();
}

template<typename T>
ClassBase* Group<T>::clone() const {
    return new Group<typename remove_const<T>::type>(cloneElems_());
}

template<typename T>
Group<typename remove_const<T>::type> Group<T>::newGroup() const {
    return Group<typename remove_const<T>::type>(cloneElems_());
}

template<typename T> template<typename T2>
bool Group<T>::isSimilar(const Group<T2>& rhs) const {
    if (this->size() != rhs.size()) {
        return false;
    }
    for (UInt i = 0; i < this->size(); i++) {
        if (!this->element_[i]->isSimilar(*rhs(i))) {
            return false;
        }
    }
    return true;
}

template<typename T>
Group<T>& Group<T>::operator=(Group<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    destruct();
    construct();
    Group<T>::add(rhs);
    return *this;
}

template<typename T> template<typename T2>
Group<T>& Group<T>::operator=(Group<T2>& rhs) {
    static_assert(is_const<T2>::value ? is_const<T>::value : true,
                  "Template parameter must be const");
    destruct();
    construct();
    Group<T>::add(rhs);
    return *this;
}

template<typename T> template<typename T2>
Group<T>& Group<T>::operator=(const Group<T2>& rhs) {
    static_assert(is_const<T>::value, "Template parameter must be const");
    destruct();
    construct();
    Group<T>::add(rhs);
    return *this;
}

template<typename T>
Group<T>& Group<T>::operator=(Group<T>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    destruct();
    construct();
    Group<T>::add(std::move(rhs));
    return *this;
}

template<typename T> template<typename T2>
Group<T>& Group<T>::operator=(Group<T2>&& rhs) {
    static_assert(is_const<T2>::value ? is_const<T>::value : true,
                  "Template parameter must be const");
    destruct();
    construct();
    Group<T>::add(std::move(rhs));
    return *this;
}

template<typename T>
void Group<T>::clear() {
    for (UInt i = 0; i < size(); i++) {
        const ClassBase* elem = get(i)->castTo<ClassBase>();
        if (SuperGroup::elems[elem].use_count() == 2) {
            SuperGroup::elems.erase(elem);
        }
    }
    element_.clear();
    set_.clear();
}

template<typename T> template<typename T2>
bool Group<T>::emptyOf() const {
    return (sizeOf<T2>() == 0);
}

template<typename T> template<typename T2>
UInt Group<T>::sizeOf() const {
    UInt res = 0;
    for (UInt i = 0; i < size(); i++) {
        if(element_[i]->template is<T2>()) {
            res++;
        }
    }
    return res;
}

template<typename T> template<typename T2>
Group<typename conditional<is_const<T>::value, const T2, T2>::type>
    Group<T>::getGroupOf() {

    return
        Group<typename conditional<is_const<T>::value, const T2, T2>::type>(
            getElemsOf_<T2>());
}

template<typename T> template<typename T2>
Group<const T2> Group<T>::getGroupOf() const {
    return Group<const T2>(getElemsOf_<T2>());
}

template<typename T> template<typename T2>
Group<T> Group<T>::getGroupNotOf() {
    return Group<T>(getElemsNotOf_<T2>());
}

template<typename T> template<typename T2>
Group<const T> Group<T>::getGroupNotOf() const {
    return Group<const T>(getElemsNotOf_<T2>());
}

template<typename T> template<class T2>
Group<typename conditional<is_const<T>::value, const T2, T2>::type>
    Group<T>::getGroupOfOnly() {

    return getGroupWith(getElemsWith_<T2>()).template getGroupOf<T2>();
}

template<typename T> template<class T2>
Group<const T2> Group<T>::getGroupOfOnly() const {
    return getGroupWith(getElemsWith_<T2>()).template getGroupOf<T2>();
}

template<typename T> template<class T2>
Group<T> Group<T>::getGroupNotOfOnly() {

    return getGroupWithout(getElemsWith_<T2>());
}

template<typename T> template<class T2>
Group<const T> Group<T>::getGroupNotOfOnly() const {
    return getGroupWithout(getElemsWith_<T2>());
}

template<typename T>
Group<T> Group<T>::getGroupWith(const UInt elem) {
    vector<UInt> aux;
    aux.push_back(elem);
    return getGroupWith(aux);
}

template<typename T>
Group<T> Group<T>::getGroupWith(const vector<UInt>& elems) {
    return Group<T>(getElemsWith_(elems));
}

template<typename T>
Group<const T> Group<T>::getGroupWith(const UInt elem) const {
    vector<UInt> aux;
    aux.push_back(elem);
    return getGroupWith(aux);
}

template<typename T>
Group<const T> Group<T>::getGroupWith(const vector<UInt>& elems) const {
    return Group<const T>(getElemsWith_(elems));
}

template<typename T>
Group<T> Group<T>::getGroupWithout(const UInt elem) {
    vector<UInt> aux;
    aux.push_back(elem);
    return getGroupWithout(aux);
}

template<typename T>
Group<T> Group<T>::getGroupWithout(const vector<UInt>& elems) {
    return Group<T>(getElemsWithout_(elems));
}

template<typename T>
Group<const T> Group<T>::getGroupWithout(const UInt elem) const {
    vector<UInt> aux;
    aux.push_back(elem);
    return getGroupWith(aux);
}

template<typename T>
Group<const T> Group<T>::getGroupWithout(const vector<UInt>& elems) const {
    return Group<const T>(getElemsWithout_(elems));
}

template<typename T> template<typename T2>
T* Group<T>::add(T2* newElem) {
    static_assert(is_const<T2>::value? is_const<T>::value : true,
                  "Template parameter must be const");
    vector<T2*> aux;
    aux.push_back(newElem);
    vector<T*> res = add(aux);
    if (!res.empty()) {
        return res[0];
    }
    return NULL;
}

template<typename T> template<typename T2>
vector<T*> Group<T>::add(const vector<T2*>& newElems) {
    static_assert(is_const<T2>::value? is_const<T>::value : true,
                  "Template parameter must be const");
    vector<T*> res = preprocess(compatElems_(newElems));
    UInt lastSize = size();
    for (UInt i = 0; i < res.size(); i++) {
        const ClassBase* elem = res[i]->castTo<ClassBase>();
        if (set_.count(res[i]) == 0) {
            if (SuperGroup::elems.count(elem) == 0) {
                SuperGroup::elems[elem] =
                    shared_ptr<ClassBase>(const_cast<ClassBase*>(elem));
            }
            shared_ptr<T> newSharedElem =
                dynamic_pointer_cast<T>(SuperGroup::elems[elem]);
            element_.push_back(newSharedElem);
            set_.insert(newSharedElem.get());
        }
    }
    postprocess(lastSize);
    return res;
}

template<typename T> template<typename T2>
vector<T*> Group<T>::add(Group<T2>& rhs) {
    static_assert(is_const<T2>::value? is_const<T>::value : true,
                  "Template parameter must be const");
    vector<T*> res;
    UInt lastSize = size();
    for (UInt i = 0; i < rhs.size(); i++) {
        if (rhs(i)->template is<T>()) {
            shared_ptr<T> newElem = dynamic_pointer_cast<T>(rhs.element_[i]);
            if (set_.count(newElem.get()) == 0) {
                element_.push_back(newElem);
                set_.insert(newElem.get());
                res.push_back(newElem.get());
            }
        }
    }
    postprocess(lastSize);
    return res;
}

template<typename T> template<typename T2>
vector<T*> Group<T>::add(const Group<T2>& rhs) {
    static_assert(is_const<T>::value, "Template parameter must be const");
    vector<T*> res;
    UInt lastSize = size();
    for (UInt i = 0; i < rhs.size(); i++) {
        if (rhs(i)->template is<T>()) {
            shared_ptr<T> newElem = dynamic_pointer_cast<T>(rhs.element_[i]);
            if (set_.count(newElem.get()) == 0) {
                element_.push_back(newElem);
                set_.insert(newElem.get());
                res.push_back(newElem.get());
            }
        }
    }
    postprocess(lastSize);
    return res;
}

template<typename T> template<typename T2>
vector<T*> Group<T>::add(Group<T2>&& rhs) {
    static_assert(is_const<T2>::value? is_const<T>::value : true,
                  "Template parameter must be const");
    vector<T*> res;
    UInt lastSize = size();
    for (UInt i = 0; i < rhs.size(); i++) {
        if (rhs(i)->template is<T>()) {
            shared_ptr<T> newElem = dynamic_pointer_cast<T>(rhs.element_[i]);
            if (set_.count(newElem.get()) == 0) {
                element_.push_back(newElem);
                set_.insert(newElem.get());
                res.push_back(newElem.get());
            }
        }
    }
    postprocess(lastSize);
    return res;
}

template<typename T>
void Group<T>::remove(const UInt elemPos) {
    vector<UInt> aux;
    aux.push_back(elemPos);
    remove(aux);
}

template<typename T>
void Group<T>::remove(const vector<UInt>& elems_) {
    set<UInt> elems(elems_.begin(), elems_.end());
    vector<shared_ptr<T>> newElems;
    newElems.reserve(size());
    for (UInt i = 0; i < size(); i++) {
        if (elems.count(i) == 0) {
            newElems.push_back(element_[i]);
        } else {
            const ClassBase* elem = get(i)->castTo<ClassBase>();
            if (SuperGroup::elems[elem].use_count() == 2) {
                SuperGroup::elems.erase(elem);
            }
            set_.erase(get(i));
        }
    }
    element_ = newElems;
}

template<typename T>
void Group<T>::printInfo() const {
    for (UInt i = 0; i < size(); i++) {
        element_[i]->printInfo();
    }
}

template<typename T>
void Group<T>::construct() {

}

template<typename T>
void Group<T>::destruct() {
    clear();
}

template<typename T>
vector<T*> Group<T>::preprocess(const vector<T*>& elems) const {
    set<const T*> auxSet;
    vector<T*> res;
    res.reserve(elems.size());
    for (UInt i = 0; i < elems.size(); i++) {
        if ((set_.count(elems[i]) == 0) && (auxSet.count(elems[i]) == 0)) {
            res.push_back(elems[i]);
            auxSet.insert(elems[i]);
        }
    }
    return res;
}

template<typename T>
void Group<T>::postprocess(const UInt i) {

}

template<typename T> template<typename T2>
vector<typename add_pointer<typename conditional<is_const<T>::value,
                                       const T2, T2>::type>::type>
    Group<T>::getElemsOf_() const {

    typedef
        typename conditional<is_const<T>::value, const T2, T2>::type condT2;
    typedef
        typename add_pointer<
            typename conditional<is_const<T>::value,
                                 const T2, T2>::type>::type ptrT2;

    vector<ptrT2> res;
    res.reserve(size());
    for (UInt i = 0; i < size(); i++) {
        if(element_[i]->template is<condT2>()) {
            res.push_back(element_[i]->castTo<condT2>());
        }
    }
    return res;
}

template<typename T> template<typename T2>
vector<T*> Group<T>::getElemsNotOf_() const {
    vector<T*> res;
    res.reserve(size());
    for (UInt i = 0; i < size(); i++) {
        if(!element_[i]->template is<T2>()) {
            res.push_back(element_[i].get());
        }
    }
    return res;
}

template<typename T> template<class T2>
vector<UInt> Group<T>::getElemsWith_() const {
    vector<UInt> elems;
    elems.reserve(this->size());
    for(UInt i = 0; i < this->size(); i++) {
        if (typeid(*this->get(i)) == typeid(T2)) {
            elems.push_back(i);
        }
    }
    return elems;
}

template<typename T>
vector<T*> Group<T>::getElemsWith_(const vector<UInt>& e) const {
    set<UInt> elems(e.begin(), e.end());
    vector<T*> res;
    res.reserve(size());
    for (set<UInt>::iterator it = elems.begin(); it != elems.end(); ++it) {
        res.push_back(element_[*it].get());
    }
    return res;
}

template<typename T>
vector<T*> Group<T>::getElemsWithout_(const vector<UInt>& e) const {
    set<UInt> elems(e.begin(), e.end());
    vector<T*> res;
    for (UInt i = 0; i < size(); i++) {
        if (elems.count(i) == 0) {
            res.push_back(element_[i].get());
        }
    }
    return res;
}

template<typename T> template<typename T2>
vector<T*> Group<T>::compatElems_(const vector<T2*>& elems) {
    vector<T*> res;
    res.reserve(elems.size());
    for(UInt i = 0; i < elems.size(); i++) {
        if (elems[i]->template is<T>()) {
            res.push_back(elems[i]->castTo<T>());
        }
    }
    return res;
}

template<typename T>
vector<typename add_pointer<typename remove_const<T>::type>::type>
    Group<T>::cloneElems_() const {

    vector<typename add_pointer<typename remove_const<T>::type>::type> res;
    res.reserve(element_.size());
    for (UInt i = 0; i < size(); i++) {
        res.push_back(element_[i]->cloneTo<T>());
    }
    return res;
}
