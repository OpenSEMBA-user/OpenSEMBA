#include "GroupId.h"

template<typename T, class Id>
ClassBase* GroupId<T, Id>::clone() const {
    return new GroupId<typename remove_const<T>::type, Id>(
                   this->newGroup());
}

template<typename T, class Id>
bool GroupId<T, Id>::existId(const Id id) const {
    return (mapId_.count(id) != 0);
}

template<typename T, class Id>
T* GroupId<T, Id>::get(const Id id) {
    if(mapId_.count(id) != 0)
        return this->get(mapId_.at(id));

    cerr << endl << "ERROR @ GroupId::get():"
         << "Inexistent Id: " << id << endl;
    assert(false);
    return NULL;
}

template<typename T, class Id>
const T* GroupId<T, Id>::get(const Id id) const {
    if(mapId_.count(id) != 0)
        return this->get(mapId_.at(id));
    
    cerr << endl << "ERROR @ GroupId::get():"
         << "Inexistent Id: " << id << endl;
    assert(false);
    return NULL;
}

template<typename T, class Id>
vector<Id> GroupId<T, Id>::getIds() const {
    vector<Id> ids;
    for (UInt i = 0; i < this->size(); i++) {
        ids.push_back(this->get(i)->getId());
    }
    return ids;
}

template<typename T, class Id>
GroupId<T, Id> GroupId<T, Id>::getGroupWith(const Id& id) {
    vector<Id> aux;
    aux.push_back(id);
    return getGroupWith(aux);
}

template<typename T, class Id>
GroupId<T, Id> GroupId<T, Id>::getGroupWith(const vector<Id>& ids) {
    return Group<T>::getGroupWith(getElemsWith_(ids));
}

template<typename T, class Id>
GroupId<const T, Id> GroupId<T, Id>::getGroupWith(const Id& id) const {
    vector<Id> aux;
    aux.push_back(id);
    return getGroupWith(aux);
}

template<typename T, class Id>
GroupId<const T, Id> GroupId<T, Id>::getGroupWith(
        const vector<Id>& ids) const {
    return Group<T>::getGroupWith(getElemsWith_(ids));
}

template<typename T, class Id>
GroupId<T, Id> GroupId<T, Id>::getGroupWithout(const Id& id) {
    vector<Id> aux;
    aux.push_back(id);
    return getGroupWithout(aux);
}

template<typename T, class Id>
GroupId<T, Id> GroupId<T, Id>::getGroupWithout(const vector<Id>& ids) {
    return Group<T>::getGroupWithout(getElemsWith_(ids));
}

template<typename T, class Id>
GroupId<const T, Id> GroupId<T, Id>::getGroupWithout(const Id& id) const {
    vector<Id> aux;
    aux.push_back(id);
    return getGroupWithout(aux);
}

template<typename T, class Id>
GroupId<const T, Id> GroupId<T, Id>::getGroupWithout(
        const vector<Id>& ids) const {

    return Group<T>::getGroupWithout(getElemsWith_(ids));
}

template<typename T, class Id> template<typename T2>
T* GroupId<T, Id>::add(T2* newElem, bool newId) {
    if (newId) {
        newElem->setId(++this->lastId_);
    }
    return Group<T>::add(newElem);
}

template<typename T, class Id> template<typename T2>
vector<T*> GroupId<T, Id>::add(vector<T2*>& newElems, bool newId) {
    if (newId) {
        for (UInt i = 0; i < newElems.size(); i++) {
            newElems[i]->setId(++this->lastId_);
        }
    }
    return Group<T>::add(newElems);
}

template<typename T, class Id> template<typename T2>
vector<T*> GroupId<T, Id>::add(Group<T2>& rhs) {
    return Group<T>::add(rhs);
}

template<typename T, class Id> template<typename T2>
vector<T*> GroupId<T, Id>::add(const Group<T2>& rhs) {
    return Group<T>::add(rhs);
}

template<typename T, class Id> template<typename T2>
vector<T*> GroupId<T, Id>::add(Group<T2>&& rhs) {
    return Group<T>::add(std::move(rhs));
}

template<typename T, class Id>
void GroupId<T, Id>::remove(const UInt elem) {
    vector<UInt> aux;
    aux.push_back(elem);
    remove(aux);
}

template<typename T, class Id>
void GroupId<T, Id>::remove(const vector<UInt>& elems) {
    for (UInt i = 0; i < elems.size(); i++) {
        mapId_.erase(this->get(i)->getId());
    }
    Group<T>::remove(elems);
}

template<typename T, class Id>
void GroupId<T, Id>::remove(const Id id) {
    vector<Id> aux;
    aux.push_back(id);
    remove(aux);
}

template<typename T, class Id>
void GroupId<T, Id>::remove(const vector<Id>& ids) {
    for (UInt i = 0; i < ids.size(); i++) {
        mapId_.erase(ids[i]);
    }
    Group<T>::remove(getElemsWith_(ids));
}

template<typename T, class Id>
void GroupId<T, Id>::construct() {
    Group<T>::construct();
    lastId_ = Id(0);
    mapId_.clear();
}

template<typename T, class Id>
void GroupId<T, Id>::destruct() {
    lastId_ = Id(0);
    mapId_.clear();
    Group<T>::destruct();
}

template<typename T, class Id>
vector<T*> GroupId<T, Id>::preprocess(const vector<T*>& v) const {
    vector<T*> res = Group<T>::preprocess(v);
    map<Id, UInt> map = mapId_;
    for (typename vector<T*>::iterator
         it = res.begin(); it != res.end(); it++) {

        if (((*it)->getId() == 0) || (map.count((*it)->getId()) == 1)) {
            it = res.erase(it);
            it--;
        } else {
            map[(*it)->getId()] = 1;
        }
    }
    return res;
}

template<typename T, class Id>
void GroupId<T, Id>::postprocess(const UInt fistStep) {
    Group<T>::postprocess(fistStep);
    for(UInt i = fistStep; i < this->size(); i++) {
        if (this->get(i)->getId() > this->lastId_)
            lastId_ = this->get(i)->getId();

        if (this->get(i)->getId() == 0) {
            cerr << endl << "ERROR @ GroupId::buildMapId():"
                 << "Element with id = 0" << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }

        if (mapId_.count(this->get(i)->getId()) == 0) {
            mapId_[this->get(i)->getId()] = i;
        } else {
            cerr << endl << "ERROR @ GroupId::buildMapId():"
                 << "Duplicated Ids" << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
    }
}

template<typename T, class Id>
vector<UInt> GroupId<T, Id>::getElemsWith_(const vector<Id>& ids) const {
    vector<UInt> elems;
    elems.reserve(ids.size());
    for(UInt i = 0; i < ids.size(); i++) {
        if (mapId_.count(ids[i]) == 1) {
            elems.push_back(mapId_.at(ids[i]));
        }
    }
    return elems;
}
