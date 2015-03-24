#ifndef GROUPID_H_
#include "GroupId.h"
#endif

template<typename T, class Id>
GroupId<T, Id>::GroupId() {
    clearMapId();
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>::GroupId(const vector<T2*>& elems)
:   Group<T>(elems) {

    clearMapId();
    buildMapId();
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>::GroupId(const vector<T2*>& elems, bool ownership)
:   Group<T>(elems, ownership) {

    clearMapId();
    buildMapId();
}

template<typename T, class Id>
GroupId<T, Id>::GroupId(const Group<T>& rhs)
:   Group<T>(rhs) {

    clearMapId();
    buildMapId();
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>::GroupId(const Group<T2>& rhs)
:   Group<T>(rhs) {

    clearMapId();
    buildMapId();
}

template<typename T, class Id>
GroupId<T, Id>::~GroupId() {
    clearMapId();
}

template<typename T, class Id>
GroupId<T, Id>& GroupId<T, Id>::operator=(const Group<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    clearMapId();
    Group<T>::operator=(rhs);
    buildMapId();

    return *this;
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>& GroupId<T, Id>::operator=(const Group<T2>& rhs) {
    clearMapId();
    Group<T>::operator=(rhs);
    buildMapId();

    return *this;
}

template<typename T, class Id>
bool GroupId<T, Id>::existId(const Id id) const {
    return (mapId_.count(id) != 0);
}

//template<typename T, class Id>
//T* GroupId<T, Id>::getPtrToId(const Id id) {
//    if(mapId_.count(id) != 0)
//        return this->element_[mapId_.at(id)];
//
//    cerr << endl << "ERROR @ GroupId::getPtrToId():"
//         << "Inexistent Id: " << id << endl;
//    assert(false);
//    exit(EXIT_FAILURE);
//    return NULL;
//}

template<typename T, class Id>
const T* GroupId<T, Id>::getPtrToId(const Id id) const {
    if(mapId_.count(id) != 0)
        return this->element_[mapId_.at(id)];
    
    cerr << endl << "ERROR @ GroupId::getPtrToId():"
         << "Inexistent Id: " << id << endl;
    assert(false);
    exit(EXIT_FAILURE);
    return NULL;
}

//template<typename T, class Id> template<typename T2>
//vector<Id> GroupId<T, Id>::getIdsOf() const {
//    vector<Id> ids;
//    for (UInt i = 0; i < this->size(); i++) {
//        if(this->element_[i]->template is<T2>()) {
//            ids.push_back(this->element_[i]->getId());
//        }
//    }
//    return ids;
//}

template<typename T, class Id>
GroupId<T, Id> GroupId<T, Id>::get(const Id& id) const {
    vector<Id> aux;
    aux.push_back(id);
    return get(aux);
}

template<typename T, class Id>
GroupId<T, Id> GroupId<T, Id>::get(const vector<Id>& ids) const {
    vector<T*> elems;
    elems.resize(ids.size());
    for(UInt i = 0; i < ids.size(); i++) {
        if (mapId_.count(ids[i]) == 0) {
            cerr << endl << "ERROR @ GroupId::get():"
                 << "Inexistent Id: " << ids[i] << endl;
            assert(false);
            exit(EXIT_FAILURE);
        } else {
            elems[i] = this->element_[mapId_.at(ids[i])];
        }
    }
    return GroupId<T, Id>(elems, false);
}

template<typename T, class Id> template<typename T2>
void GroupId<T, Id>::add(T2* newElem, bool newId) {
    vector<T2*> aux;
    aux.push_back(newElem);
    add(aux, newId);
}

template<typename T, class Id> template<typename T2>
void GroupId<T, Id>::add(vector<T2*>& newElems, bool newId) {
    for (UInt i = 0; i < newElems.size(); i++) {
        if(!newElems[i]->template is<T>()) {
            continue;
        } else if(newId) {
            newElems[i]->setId(++this->lastId_);
        }
    }
    Group<T>::add(newElems);
    buildMapId();
}

template<typename T, class Id> template<typename T2>
void GroupId<T, Id>::add(const Group<T2>& rhs) {
    Group<T>::add(rhs);
    buildMapId();
}

template<typename T, class Id>
void GroupId<T, Id>::buildMapId() {
    for(UInt i = lastPosMap_; i < this->size(); i++) {
        if (this->element_[i]->getId() > this->lastId_)
            lastId_ = this->element_[i]->getId();

        if (this->element_[i]->getId() == 0) {
            cerr << endl << "ERROR @ GroupId::buildMapId():"
                 << "Element with id = 0" << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }

        if (mapId_.count(this->element_[i]->getId()) == 0) {
            mapId_[this->element_[i]->getId()] = i;
        } else {
            cerr << endl << "ERROR @ GroupId::buildMapId():"
                 << "Duplicated Ids" << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
    }
    lastPosMap_ = this->size();
}

template<typename T, class Id>
void GroupId<T, Id>::clearMapId() {
    lastId_ = Id(0);
    lastPosMap_ = 0;
    mapId_.clear();
}
