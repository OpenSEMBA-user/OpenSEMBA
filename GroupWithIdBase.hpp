#ifndef GROUPWITHIDBASE_H_
#include "GroupWithIdBase.h"
#endif

template<typename T, class Id>
GroupWithIdBase<T, Id>::GroupWithIdBase()
:   lastId_(0) {

}

template<typename T, class Id>
GroupWithIdBase<T, Id>::GroupWithIdBase(const vector<T*>& elems)
:   GroupBase<T, Id>(elems),
    lastId_(0) {

    for(unsigned i = 0; i < this->size(); i++) {
        if (this->element_[i]->getId() > this->lastId_)
            lastId_ = this->element_[i]->getId();
        
        if (lastId_ == 0) {
            cerr << "GroupWithIdBase::GroupWithIdBase():"
                 << "Element with id = 0" << endl;
        }
        
        if (mapId_.count(this->element_[i]->getId()) == 0) {
            mapId_[this->element_[i]->getId()] = i;
        } else {
            cerr << "GroupWithIdBase::GroupWithIdBase():"
                 << "Duplicated Ids" << endl;
            assert(false);
            exit(1);
        }
    }
}

template<typename T, class Id>
GroupWithIdBase<T, Id>::GroupWithIdBase(const GroupWithIdBase<T, Id>& rhs)
:   GroupBase<T, Id>(rhs),
    lastId_(rhs.lastId_),
    mapId_ (rhs.mapId_) {

}

template<typename T, class Id>
GroupWithIdBase<T, Id>::~GroupWithIdBase() {

}

template<typename T, class Id>
GroupWithIdBase<T, Id>&
GroupWithIdBase<T, Id>::operator=(const GroupWithIdBase<T, Id>& rhs) {
    if(this == &rhs)
        return *this;

    GroupBase<T, Id>::operator=(rhs);

    this->lastId_ = rhs.lastId_;
    this->mapId_  = rhs.mapId_;

    return *this;
}

template<typename T, class Id>
const T* GroupWithIdBase<T, Id>::getPtrToId(const Id id) const {
    if(mapId_.count(id) != 0)
        return this->element_[mapId_.at(id)];
    
    return NULL;
}

template<typename T, class Id> template<typename T2>
GroupWithIdBase<T2, Id> GroupWithIdBase<T, Id>::getGroupOf() const {
    vector<T2*> elems;
    for (unsigned i = 0; i < this->size(); i++)
        if(this->element_[i]->template isOf<T2>())
            elems.push_back(this->element_[i]->template castTo<T2>());

    return GroupWithIdBase<T2, Id>(elems);
}

template<typename T, class Id>
Id GroupWithIdBase<T, Id>::add(const T* newElem) {
    vector<Id> ids;
    vector<T*> aux;
    T* newElem_ = newElem->clone()->template castTo<T>();
    aux.push_back(newElem_);
    ids = add(aux);
    delete newElem_;
    return ids[0];
}

template<typename T, class Id>
vector<Id> GroupWithIdBase<T, Id>::add(const vector<T*>& newElems) {
    vector<Id> newIds;
    this->element_.reserve(this->element_.size() + newElems.size());
    newIds.reserve(newElems.size());
    for (unsigned i = 0; i < newElems.size(); i++) {
        this->element_.push_back(
            newElems[i]->clone(++this->lastId_)->template castTo<T>());
        newIds.push_back(this->lastId_);
        if(mapId_.count(this->element_.back()->getId()) == 0) {
            mapId_[this->element_.back()->getId()] = this->element_.size()-1;
        } else {
            cerr << "GroupWithIdBase::add():"
                 << "Duplicated Ids" << endl;
            assert(false);
            exit(1);
        }
    }
    return newIds;
}
