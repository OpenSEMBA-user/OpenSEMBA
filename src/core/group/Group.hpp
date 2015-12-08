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

#include "Group.h"

#include <set>
#include <type_traits>

namespace SEMBA {
namespace Group {

template<typename T>
Base<T>::Base() {

}

template<typename T>
Base<T>::~Base() {

}

template<typename T>
bool Base<T>::empty() const {
    return (size() == 0);
}

template<typename T>
T* Base<T>::operator()(const std::size_t pos) {
    return get(pos);
}

template<typename T>
const T* Base<T>::operator()(const std::size_t pos) const {
    return get(pos);
}

template<typename T> template<typename T2>
Group<T>::Group(T2* elem) {
    element_.reserve(1);
    if (dynamic_cast<const T*>(elem) != NULL) {
        element_.push_back(getSharedPtr(elem));
    }
}

template<typename T> template<typename T2>
Group<T>::Group(const std::vector<T2*>& elems) {
    element_.reserve(elems.size());
    for (std::size_t i = 0; i < elems.size(); i++) {
        if (dynamic_cast<const T*>(elems[i]) != NULL) {
            element_.push_back(getSharedPtr(elems[i]));
        }
    }
}

template<typename T> template<typename T2>
Group<T>::Group(Group<T2>& rhs) {
    element_.reserve(rhs.size());
    for (std::size_t i = 0; i < rhs.size(); i++) {
        if (dynamic_cast<const T*>(rhs(i)) != NULL) {
            element_.push_back(getSharedPtr(rhs(i)));
        }
    }
}

template<typename T> template<typename T2>
Group<T>::Group(Group<T2>&& rhs) {
    element_.reserve(rhs.size());
    for (std::size_t i = 0; i < rhs.size(); i++) {
        if (dynamic_cast<const T*>(rhs(i)) != NULL) {
            element_.push_back(std::move(getSharedPtr(rhs(i))));
        }
    }
}

template<typename T> template<typename T2>
Group<T>::Group(const Group<T2>& rhs) {
    static_assert(std::is_const<T>::value, "Template parameter must be const");
    element_.reserve(rhs.size());
    for (std::size_t i = 0; i < rhs.size(); i++) {
        if (dynamic_cast<const T*>(rhs(i)) != NULL) {
            element_.push_back(getSharedPtr(rhs(i)));
        }
    }
}

template<typename T>
Group<T>::Group(Group& rhs) {
    element_ = rhs.element_;
}

template<typename T>
Group<T>::Group(Group&& rhs) {
    element_ = std::move(rhs.element_);
}

template<typename T>
Group<T>& Group<T>::operator=(Group& rhs) {
    if (this == &rhs) {
        return *this;
    }
    clear();
    element_ = rhs.element_;
    return *this;
}

template<typename T>
Group<T>& Group<T>::operator=(Group&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    clear();
    element_ = std::move(rhs.element_);
    return *this;
}

template<typename T> template<typename T2>
bool Group<T>::emptyOf() const {
    return (sizeOf<T2>() == 0);
}

template<typename T> template<typename T2>
std::size_t Group<T>::sizeOf() const {
    std::size_t res = 0;
    for (std::size_t i = 0; i < this->size(); i++) {
        if(this->get(i)->template is<T2>()) {
            res++;
        }
    }
    return res;
}

template<typename T>
T* Group<T>::get(const std::size_t i) {
    return element_[i].get();
}

template<typename T>
const T* Group<T>::get(const std::size_t i) const {
    return element_[i].get();
}

template<typename T>
Group<T> Group<T>::get(const std::vector<std::size_t>& pos_) {
    std::set<std::size_t> pos(pos_.begin(), pos_.end());
    Group<T> res;
    res.reserve(this->size());
    for (std::set<std::size_t>::const_iterator
         it = pos.begin(); it != pos.end(); ++it) {
        res.add(element_[*it].get());
    }
    return res;
}

template<typename T>
Group<const T> Group<T>::get(const std::vector<std::size_t>& pos_) const {
    std::set<std::size_t> pos(pos_.begin(), pos_.end());
    Group<T> res;
    res.reserve(this->size());
    for (std::set<std::size_t>::const_iterator
         it = pos.begin(); it != pos.end(); ++it) {
        res.add(element_[*it].get());
    }
    return res;
}

template<typename T> template<typename T2>
Group<typename std::conditional<std::is_const<T>::value, const T2, T2>::type>
        Group<T>::getOf() {
    return
        Group<
            typename std::conditional<
                std::is_const<T>::value,
                const T2, T2>::type>(this->get(getElemsOf_<T2>()));
}

template<typename T> template<typename T2>
Group<const T2> Group<T>::getOf() const {
    return Group<const T2>(this->get(getElemsOf_<T2>()));
}

template<typename T> template<class T2>
Group<typename std::conditional<std::is_const<T>::value, const T2, T2>::type>
        Group<T>::getOfOnly() {
    return
        Group<
            typename std::conditional<
                std::is_const<T>::value,
                const T2, T2>::type>(this->get(getElemsOfOnly_<T2>()));
}

template<typename T> template<class T2>
Group<const T2> Group<T>::getOfOnly() const {
    return Group<const T2>(this->get(getElemsOfOnly_<T2>()));
}

template<typename T>
void Group<T>::set(const std::size_t i, T* elem) {
    element_[i] = getSharedPtr(elem);
}

template<typename T> template<typename T2>
Group<T> Group<T>::add(T2* elem) {
    return add(Group(elem));
}

template<typename T> template<typename T2>
Group<T> Group<T>::add(const std::vector<T2*>& elems) {
    return add(Group(elems));
}

template<typename T> template<typename T2>
Group<T> Group<T>::add(Group<T2>& rhs) {
    return add(Group<T>(rhs));
}

template<typename T> template<typename T2>
Group<T> Group<T>::add(const Group<T2>& rhs) {
    static_assert(std::is_const<T>::value, "Template parameter must be const");
    return add(Group<T>(rhs));
}

template<typename T>
Group<T> Group<T>::add(Group& rhs) {
    element_.insert(element_.end(), rhs.element_.begin(), rhs.element_.end());
    return rhs;
}

template<typename T>
Group<T> Group<T>::add(Group&& rhs) {
    element_.insert(element_.end(), rhs.element_.begin(), rhs.element_.end());
    return rhs;
}

template<typename T>
void Group<T>::remove(const std::size_t& pos) {
    std::vector<std::size_t> aux;
    aux.push_back(pos);
    remove(aux);
}

template<typename T>
void Group<T>::remove(const std::vector<std::size_t>& pos_) {
    std::set<std::size_t> pos(pos_.begin(), pos_.end());
    Group<T> aux;
    aux.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if (!pos.count(i)) {
            aux.add(element_[i].get());
        }
    }
    operator=(std::move(aux));
}

template<typename T> template<typename T2>
std::vector<std::size_t> Group<T>::getElemsOf_() const {
    std::vector<std::size_t> res;
    res.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if(this->get(i)->template is<T2>()) {
            res.push_back(i);
        }
    }
    return res;
}

template<typename T> template<typename T2>
std::vector<std::size_t> Group<T>::getElemsNotOf_() const {
    std::vector<std::size_t> res;
    res.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if(!this->get(i)->template is<T2>()) {
            res.push_back(i);
        }
    }
    return res;
}

template<typename T> template<typename T2>
std::vector<std::size_t> Group<T>::getElemsOfOnly_() const {
    std::vector<std::size_t> res;
    res.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if(typeid(*this->get(i)) == typeid(T2)) {
            res.push_back(i);
        }
    }
    return res;
}

template<typename T> template<typename T2>
std::vector<std::size_t> Group<T>::getElemsNotOfOnly_() const {
    std::vector<std::size_t> res;
    res.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if(typeid(*this->get(i)) != typeid(T2)) {
            res.push_back(i);
        }
    }
    return res;
}

template<typename T> template<typename T2>
std::shared_ptr<T> Group<T>::getSharedPtr(T2* elem) const {
    try {
        return std::dynamic_pointer_cast<T>(elem->shared_from_this());
    } catch (const std::bad_weak_ptr&) {
        return std::shared_ptr<T>(dynamic_cast<T*>(elem));
    }
}

} /* namespace Group */
} /* namespace SEMBA */
