#ifndef GROUPID_H_
#include "GroupId.h"
#endif

template<typename T, class Id>
GroupId<T, Id>::GroupId()
:   lastId_(0) {

}

template<typename T, class Id>
GroupId<T, Id>::GroupId(const vector<T*>& elems)
:   Group<T>(elems),
    lastId_(0) {

    buildMapId();
}

template<typename T, class Id>
GroupId<T, Id>::GroupId(const Group<T>& rhs)
:   Group<T>(rhs) {

    buildMapId();
}

template<typename T, class Id>
GroupId<T, Id>::~GroupId() {

}

template<typename T, class Id>
GroupId<T, Id>&
GroupId<T, Id>::operator=(const Group<T>& rhs) {
    if(this == &rhs)
        return *this;

    Group<T>::operator=(rhs);

    buildMapId();

    return *this;
}

template<typename T, class Id>
const T* GroupId<T, Id>::getPtrToId(const Id id) const {
    if(mapId_.count(id) != 0)
        return this->element_[mapId_.at(id)];
    
    return NULL;
}

template<typename T, class Id> template<typename T2>
vector<Id> GroupId<T, Id>::getIdsOf() const {
    vector<Id> ids;
    for (unsigned i = 0; i < this->size(); i++) {
        if(this->element_[i]->template is<T2>()) {
            ids.push_back(this->element_[i]->getId());
        }
    }
    return ids;
}

template<typename T, class Id>
void GroupId<T, Id>::add(T* newElem, bool newId) {
    vector<T*> aux;
    aux.push_back(newElem);
    add(aux, newId);
}

template<typename T, class Id>
void GroupId<T, Id>::add(vector<T*>& newElems, bool newId) {
    if(!this->ownership_) {
        cerr << "ERROR @ Group::add(): "
             << "Forbidden to add elements to a Group without ownership "
             << "of elements on it" << endl;
        assert(false);
        exit(1);
    }

    this->element_.reserve(this->size() + newElems.size());
    for (unsigned i = 0; i < newElems.size(); i++) {
        if(newId) {
            newElems[i]->setId(++this->lastId_);
        }
        this->element_.push_back(newElems[i]);
        if(mapId_.count(this->element_.back()->getId()) == 0) {
            mapId_[this->element_.back()->getId()] = this->size()-1;
        } else {
            cerr << "ERROR @ GroupId::add():"
                 << "Duplicated Ids" << endl;
            assert(false);
            exit(1);
        }
    }
}

template<typename T, class Id>
void GroupId<T, Id>::buildMapId() {
    for(unsigned i = 0; i < this->size(); i++) {
        if (this->element_[i]->getId() > this->lastId_)
            lastId_ = this->element_[i]->getId();

        if (lastId_ == 0) {
            cerr << "ERROR @ GroupId::buildMapId():"
                 << "Element with id = 0" << endl;
            assert(false);
            exit(1);
        }

        if (mapId_.count(this->element_[i]->getId()) == 0) {
            mapId_[this->element_[i]->getId()] = i;
        } else {
            cerr << "ERROR @ GroupId::buildMapId():"
                 << "Duplicated Ids" << endl;
            assert(false);
            exit(1);
        }
    }
}
