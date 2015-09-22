// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
#include "GroupId.h"

template<typename T, class Id>
GroupId<T,Id>::ErrorId::ErrorId(const Id& id) {
    id_ = id;
}

template<typename T, class Id>
GroupId<T,Id>::ErrorId::~ErrorId() throw() {

}

template<typename T, class Id>
Id GroupId<T,Id>::ErrorId::getId() const {
    return id_;
}

template<typename T, class Id>
GroupId<T, Id>::ErrorIdNotExists::ErrorIdNotExists(const Id& id)
:   Error(std::string("GroupId: Element with id ") + id.toStr() +
          std::string(" not exists")),
    ErrorId(id) {
}

template<typename T, class Id>
GroupId<T, Id>::ErrorIdNotExists::~ErrorIdNotExists() throw () {
}

template<typename T, class Id>
GroupId<T,Id>::ErrorIdZero::ErrorIdZero(const Id& id)
:   Error("GroupId: Element with id 0"), ErrorId(id) {

}

template<typename T, class Id>
GroupId<T,Id>::ErrorIdZero::~ErrorIdZero() throw() {

}

template<typename T, class Id>
GroupId<T,Id>::ErrorIdDuplicated::ErrorIdDuplicated(const Id& id)
:   ErrorId(id) {
    std::stringstream aux;
    aux << "GroupId: Duplicated Id " << id;
    this->setMsg(aux.str());
}

template<typename T, class Id>
GroupId<T,Id>::ErrorIdDuplicated::~ErrorIdDuplicated() throw() {

}

template<typename T, class Id>
GroupId<T,Id>::GroupId() {
    lastId_ = Id(0);
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>::GroupId(T2* elem)
:   Group<T>(elem) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>::GroupId(const std::vector<T2*>& elems)
:   Group<T>(elems) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>::GroupId(VectorPtr<T2>& rhs)
:   Group<T>(rhs) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>::GroupId(const VectorPtr<T2>& rhs)
:   Group<T>(rhs) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id>
GroupId<T,Id>::GroupId(VectorPtr<T>& rhs)
:   Group<T>(rhs) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id> template<typename T2>
GroupId<T, Id>::GroupId(VectorPtr<T2> && rhs)
:   Group<T>(std::move(rhs)) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id>
GroupId<T,Id>::GroupId(VectorPtr<T>&& rhs)
:   Group<T>(std::move(rhs)) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id>
GroupId<T,Id>::~GroupId() {

}

template<typename T, class Id>
GroupId<T,Id>* GroupId<T,Id>::clone() const {
    return new GroupId<T, Id>(this->cloneElems());
}

template<typename T, class Id>
GroupId<T,Id>& GroupId<T,Id>::operator=(VectorPtr<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<T>::operator=(rhs);
    postprocess_(0);
    return *this;
}

template<typename T, class Id>
GroupId<T,Id>& GroupId<T,Id>::operator=(VectorPtr<T>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<T>::operator=(std::move(rhs));
    postprocess_(0);
    return *this;
}

template<typename T, class Id>
void GroupId<T,Id>::clear() {
    Group<T>::clear();
    lastId_ = Id(0);
    mapId_.clear();
}

template<typename T, class Id>
bool GroupId<T,Id>::existId(const Id id) const {
    return (mapId_.count(id) != 0);
}

template<typename T, class Id>
std::vector<Id> GroupId<T,Id>::getIds() const {
    std::vector<Id> ids;
    for (UInt i = 0; i < this->size(); i++) {
        ids.push_back(this->get(i)->getId());
    }
    return ids;
}

template<typename T, class Id>
typename Reference<T*>::type GroupId<T,Id>::getId(const Id id) {
    if(mapId_.count(id) == 0) {
        throw ErrorIdNotExists(id);
    }
    return this->get(mapId_.at(id));
}

template<typename T, class Id>
typename ConstReference<T*>::type GroupId<T,Id>::getId(const Id id) const {
    if(mapId_.count(id) == 0) {
        throw ErrorIdNotExists(id);
    }
    return this->get(mapId_.at(id));
}

template<typename T, class Id>
GroupId<T,Id> GroupId<T,Id>::getId(const std::vector<Id>& ids) {
    return this->get(getElemsId_(ids));
}

template<typename T, class Id>
GroupId<const T,Id> GroupId<T,Id>::getId(const std::vector<Id>& ids) const {
    return this->get(getElemsId_(ids));
}

template<typename T, class Id>
VectorPtr<T> GroupId<T,Id>::add(VectorPtr<T>& rhs) {
    UInt lastSize = this->size();
    Group<T>::add(rhs);
    postprocess_(lastSize);
    return rhs;
}

template<typename T, class Id>
VectorPtr<T> GroupId<T,Id>::add(VectorPtr<T>&& rhs) {
    UInt lastSize = this->size();
    Group<T>::add(std::move(rhs));
    postprocess_(lastSize);
    return rhs;
}

template<typename T, class Id> template<typename T2>
VectorPtr<T> GroupId<T, Id>::addId(T2* elem) {
    return addId(VectorPtr<T>(elem));
}

template<typename T, class Id> template<typename T2>
VectorPtr<T> GroupId<T, Id>::addId(const std::vector<T2*>& elems) {
    return addId(VectorPtr<T>(elems));
}

template<typename T, class Id> template<typename T2>
VectorPtr<T> GroupId<T,Id>::addId(VectorPtr<T2>& rhs) {
    return addId(VectorPtr<T>(rhs));
}

template<typename T, class Id>
VectorPtr<T> GroupId<T,Id>::addId(VectorPtr<T>& rhs) {
    for (UInt i = 0; i < rhs.size(); i++) {
        rhs(i)->setId(++lastId_);
    }
    return add(rhs);
}

template<typename T, class Id>
VectorPtr<T> GroupId<T,Id>::addId(VectorPtr<T>&& rhs) {
    for (UInt i = 0; i < rhs.size(); i++) {
        rhs(i)->setId(++lastId_);
    }
    return add(std::move(rhs));
}

template<typename T, class Id>
void GroupId<T,Id>::remove(const UInt& pos) {
    mapId_.erase(this->get(pos)->getId());
    Group<T>::remove(pos);
}

template<typename T, class Id>
void GroupId<T,Id>::remove(const std::vector<UInt>& pos) {
    for (UInt i = 0; i < pos.size(); i++) {
        mapId_.erase(this->get(pos[i])->getId());
    }
    Group<T>::remove(pos);
}

template<typename T, class Id>
void GroupId<T,Id>::removeId(const Id id) {
    std::vector<Id> aux;
    aux.push_back(id);
    removeId(aux);
}

template<typename T, class Id>
void GroupId<T,Id>::removeId(const std::vector<Id>& ids) {
    remove(getElemsId_(ids));
}

template<typename T, class Id>
void GroupId<T,Id>::postprocess_(const UInt& firstStep) {
    for(UInt i = firstStep; i < this->size(); i++) {
        if (this->get(i)->getId() > this->lastId_) {
            lastId_ = this->get(i)->getId();
        }
        if (this->get(i)->getId() == Id(0)) {
            throw ErrorIdZero(this->get(i)->getId());
        }
        if (mapId_.count(this->get(i)->getId()) == 0) {
            mapId_[this->get(i)->getId()] = i;
        } else {
            throw ErrorIdDuplicated(this->get(i)->getId());
        }
    }
}

template<typename T, class Id>
std::vector<UInt> GroupId<T,Id>::getElemsId_(const std::vector<Id>& ids) const {
    std::vector<UInt> res;
    res.reserve(ids.size());
    for(UInt i = 0; i < ids.size(); i++) {
        if (mapId_.count(ids[i]) == 1) {
            res.push_back(mapId_.at(ids[i]));
        }
    }
    return res;
}
