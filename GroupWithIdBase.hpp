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

    element_.resize(elems.size());
    for(UInt i = 0; i < elems.size(); i++) {
        if(elems[i]->getId() > this->lastId_)
            lastId_ = elems[i]->getId();

        if(mapId_.count(elems[i]->getId()) == 0)
            mapId_[elems[i]->getId()] = i;
        else
            cerr << "GroupWithIdBase::GroupWithIdBase():"
                 << "Duplicated Ids" << endl;
    }
}

template<typename T, class Id>
GroupWithIdBase<T, Id>::GroupWithIdBase(const GroupWithIdBase<T, Id>& rhs)
:   GroupBase<T>(rhs),
    lastId_(rhs.lastId_),
    mapId_(rhs.mapId_) {

}

template<typename T, class Id>
GroupWithIdBase<T, Id>::~GroupWithIdBase() {

}

template<typename T, class Id>
GroupWithIdBase<T, Id>&
GroupWithIdBase<T, Id>::operator=(const GroupWithIdBase<T, Id>& rhs) {

    if(this == &rhs)
        return *this;

    GroupBase<E>::operator=(rhs);

    this->lastId_ = rhs.lastId_;
    this->mapId_  = rhs.mapId_;

    return *this;
}

template<typename T, class Id>
const T* GroupWithIdBase<T, Id>::getPtrToId(const Id id) const {
    if(mapId_.count(id) != 0)
        return this->element_[mapId_.at(id)];
    return nullptr;
}

template<typename T, class Id> template<typename T2>
GroupWithIdBase<T2, Id> GroupWithIdBase<T, Id>::getGroupOf() const {
    vector<T2*> elems;
    for (UInt i = 0; i < this->size(); i++)
        if(isOf<T2>(this->element_[i]))
            elems.push_back(this->element_[i]->castTo<T2>());

    return GroupWithIdBase<T2, Id>(elems);
}

template<typename T, class Id>
Id GroupWithIdBase<T, Id>::add(const T* newElem) {
    this->element_.reserve(this->element_.size() + 1);
    this->element_.push_back(newElem->clone(++this->lastId_)->castTo<T>());
    if(mapId_.count(this->element_.back()->getId()) == 0)
        mapId_[this->element_.back()->getId()] = this->element_.size()-1;
    else
        cerr << "GroupWithIdBase::add():"
             << "Duplicated Ids" << endl;

    return lastId_;
}

template<typename T, class Id>
vector<Id> GroupWithIdBase<T, Id>::add(const vector<T*>& newElems) {
    vector<Id> newIds;
    this->element_.reserve(this->element_.size() + newElems.size());
    newIds.reserve(newElems.size());
    for (UInt i = 0; i < newElems.size(); i++) {
        this->element_.push_back(
            newElems[i]->clone(++this->lastId_)->castTo<T>());
        newIds.push_back(this->lastId_);
        if(mapId_.count(this->element_.back()->getId()) == 0)
            mapId_[this->element_.back()->getId()] = this->element_.size()-1;
        else
            cerr << "GroupWithIdBase::add():"
                 << "Duplicated Ids" << endl;
    }
    return newIds;
}
