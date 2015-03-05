#ifndef GROUPWITHIDBASE_H_
#include "GroupWithIdBase.h"
#endif

template<typename T, class Id>
GroupWithIdBase<T, Id>::GroupWithIdBase()
:   lastId_(0) {

}

template<typename T, class Id>
GroupWithIdBase<T, Id>::GroupWithIdBase(const vector<T*>& elems)
:   GroupBase<T>(elems),
    lastId_(0) {

    buildMapId();
}

template<typename T, class Id>
GroupWithIdBase<T, Id>::GroupWithIdBase(const GroupBase<T>& rhs)
:   GroupBase<T>(rhs) {

    buildMapId();
}

template<typename T, class Id>
GroupWithIdBase<T, Id>::~GroupWithIdBase() {

}

template<typename T, class Id>
GroupWithIdBase<T, Id>&
GroupWithIdBase<T, Id>::operator=(const GroupBase<T>& rhs) {
    if(this == &rhs)
        return *this;

    GroupBase<T>::operator=(rhs);

    buildMapId();

    return *this;
}

template<typename T, class Id>
const T* GroupWithIdBase<T, Id>::getPtrToId(const Id id) const {
    if(mapId_.count(id) != 0)
        return this->element_[mapId_.at(id)];
    
    return NULL;
}


template<typename T, class Id> template<typename T2>
vector<Id> GroupWithIdBase<T, Id>::getVectorIdsOf() const {
    vector<Id> res;

    typedef typename map<Id, unsigned>::constant_iterator it;
    for (it = mapId_.begin(); it != mapId_.end(); ++it) {
        if(this->element_[it->second]->template isOf<T2>()) {
            res.push_back(it->first);
        }
    }

    return res;
}

template<typename T, class Id>
const T* GroupWithIdBase<T, Id>::add(T* newElem) {
    vector<const T*> res;
    vector<T*> aux;
    aux.push_back(newElem);
    res = add(aux);

    assert(res.size() == 1);
    return res[0];
}

template<typename T, class Id>
vector<const T*> GroupWithIdBase<T, Id>::add(const vector<T*>& newElems) {
    vector<const T*> res;
    this->element_.reserve(this->size() + newElems.size());
    for (unsigned i = 0; i < newElems.size(); i++) {
        this->element_.push_back(
            newElems[i]->clone(++this->lastId_)->template castTo<T>());
        res.push_back(this->element_.back());
        if(mapId_.count(this->element_.back()->getId()) == 0) {
            mapId_[this->element_.back()->getId()] = this->size()-1;
        } else {
            cerr << "ERROR @ GroupWithIdBase::add():"
                 << "Duplicated Ids" << endl;
            assert(false);
            exit(1);
        }
    }
    assert(res.size() == newElems.size());
    return res;
}

template<typename T, class Id>
void GroupWithIdBase<T, Id>::buildMapId() {
    for(unsigned i = 0; i < this->size(); i++) {
        if (this->element_[i]->getId() > this->lastId_)
            lastId_ = this->element_[i]->getId();

        if (lastId_ == 0) {
            cerr << "ERROR @ GroupWithIdBase::buildMapId():"
                 << "Element with id = 0" << endl;
            assert(false);
            exit(1);
        }

        if (mapId_.count(this->element_[i]->getId()) == 0) {
            mapId_[this->element_[i]->getId()] = i;
        } else {
            cerr << "ERROR @ GroupWithIdBase::buildMapId():"
                 << "Duplicated Ids" << endl;
            assert(false);
            exit(1);
        }
    }
}
