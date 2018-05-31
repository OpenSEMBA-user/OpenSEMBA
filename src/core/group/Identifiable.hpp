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

#include "Identifiable.h"

#include "Error.h"

#include <string>

namespace SEMBA {
namespace Group {

template<typename T, class Id>
Identifiable<T,Id>::Identifiable() {
    lastId_ = Id(0);
}

template<typename T, class Id> template<typename T2>
Identifiable<T, Id>::Identifiable(T2* elem)
:   Group<T>(elem) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id> template<typename T2>
Identifiable<T, Id>::Identifiable(const std::vector<T2*>& elems)
:   Group<T>(elems) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id> template<typename T2>
Identifiable<T, Id>::Identifiable(Group<T2>& rhs)
:   Group<T>(rhs) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id> template<typename T2>
Identifiable<T, Id>::Identifiable(const Group<T2>& rhs)
:   Group<T>(rhs) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id>
Identifiable<T,Id>::Identifiable(Group<T>& rhs)
:   Group<T>(rhs) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id> template<typename T2>
Identifiable<T, Id>::Identifiable(Group<T2> && rhs)
:   Group<T>(std::move(rhs)) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id>
Identifiable<T,Id>::Identifiable(Group<T>&& rhs)
:   Group<T>(std::move(rhs)) {
    lastId_ = Id(0);
    postprocess_(0);
}

template<typename T, class Id>
Identifiable<T,Id>::~Identifiable() {

}

template<typename T, class Id>
Identifiable<T,Id>& Identifiable<T,Id>::operator=(Group<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<T>::operator=(rhs);
    postprocess_(0);
    return *this;
}

template<typename T, class Id>
Identifiable<T,Id>& Identifiable<T,Id>::operator=(Group<T>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Group<T>::operator=(std::move(rhs));
    postprocess_(0);
    return *this;
}

template<typename T, class Id>
void Identifiable<T,Id>::clear() {
    Group<T>::clear();
    lastId_ = Id(0);
    mapId_.clear();
}

template<typename T, class Id>
bool Identifiable<T,Id>::existId(const Id id) const {
    return (mapId_.count(id) != 0);
}

template<typename T, class Id>
std::vector<Id> Identifiable<T,Id>::getIds() const {
    std::vector<Id> ids;
    for (std::size_t i = 0; i < this->size(); i++) {
        ids.push_back(this->get(i)->getId());
    }
    return ids;
}

template<typename T, class Id>
T* Identifiable<T,Id>::getId(const Id id) {
    if(mapId_.count(id) == 0) {
        throw typename Error::Id::NotExists<Id>(id);
    }
    return this->get(mapId_.at(id));
}

template<typename T, class Id>
const T* Identifiable<T,Id>::getId(const Id id) const {
    if(mapId_.count(id) == 0) {
        throw typename Error::Id::NotExists<Id>(id);
    }
    return this->get(mapId_.at(id));
}

template<typename T, class Id>
Identifiable<T,Id> Identifiable<T,Id>::getId(const std::vector<Id>& ids) {
    return this->get(getElemsId_(ids));
}

template<typename T, class Id>
Identifiable<const T,Id> Identifiable<T,Id>::getId(
        const std::vector<Id>& ids) const {
    return this->get(getElemsId_(ids));
}

template<typename T, class Id>
Group<T> Identifiable<T,Id>::add(Group<T>& rhs) {
    std::size_t lastSize = this->size();
    Group<T>::add(rhs);
    postprocess_(lastSize);
    return rhs;
}

template<typename T, class Id>
Group<T> Identifiable<T,Id>::add(Group<T>&& rhs) {
    std::size_t lastSize = this->size();
    Group<T>::add(std::move(rhs));
    postprocess_(lastSize);
    return rhs;
}

template<typename T, class Id> template<typename T2>
Group<T> Identifiable<T, Id>::addId(T2* elem) {
    return addId(Group<T>(elem));
}

template<typename T, class Id> template<typename T2>
Group<T> Identifiable<T, Id>::addId(const std::vector<T2*>& elems) {
    return addId(Group<T>(elems));
}

template<typename T, class Id> template<typename T2>
Group<T> Identifiable<T,Id>::addId(Group<T2>& rhs) {
    return addId(Group<T>(rhs));
}

template<typename T, class Id>
Group<T> Identifiable<T,Id>::addId(Group<T>& rhs) {
    for (std::size_t i = 0; i < rhs.size(); i++) {
        rhs(i)->setId(++lastId_);
    }
    return add(rhs);
}

#ifdef __GNUC__
template<typename T, class Id>
Group<T> Identifiable<T,Id>::addId(Group<T>&& rhs) {
    for (std::size_t i = 0; i < rhs.size(); i++) {
        rhs(i)->setId(++lastId_);
    }
    return add(std::move(rhs));
}
#endif

template<typename T, class Id>
void Identifiable<T,Id>::remove(const std::size_t& pos) {
    mapId_.erase(this->get(pos)->getId());
    Group<T>::remove(pos);
}

template<typename T, class Id>
void Identifiable<T,Id>::remove(const std::vector<std::size_t>& pos) {
    for (std::size_t i = 0; i < pos.size(); i++) {
        mapId_.erase(this->get(pos[i])->getId());
    }
    Group<T>::remove(pos);
}

template<typename T, class Id>
void Identifiable<T,Id>::removeId(const Id id) {
    std::vector<Id> aux;
    aux.push_back(id);
    removeId(aux);
}

template<typename T, class Id>
void Identifiable<T,Id>::removeId(const std::vector<Id>& ids) {
    remove(getElemsId_(ids));
}

template<typename T, class Id>
void Identifiable<T,Id>::postprocess_(const std::size_t& firstStep) {
    for(std::size_t i = firstStep; i < this->size(); i++) {
        if (this->get(i)->getId() > this->lastId_) {
            lastId_ = this->get(i)->getId();
        }
        if (this->get(i)->getId() == Id(0)) {
            throw typename Error::Id::Zero<Id>();
        }
        if (mapId_.count(this->get(i)->getId()) == 0) {
            mapId_[this->get(i)->getId()] = i;
        } else {
            throw typename Error::Id::Duplicated<Id>(this->get(i)->getId());
        }
    }
}

template<typename T, class Id>
std::vector<std::size_t> Identifiable<T,Id>::getElemsId_(
        const std::vector<Id>& ids) const {
    std::vector<std::size_t> res;
    res.reserve(ids.size());
    for(std::size_t i = 0; i < ids.size(); i++) {
        if (mapId_.count(ids[i]) == 1) {
            res.push_back(mapId_.at(ids[i]));
        }
    }
    return res;
}

} /* namespace Group */
} /* namespace SEMBA */
