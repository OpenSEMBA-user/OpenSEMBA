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
/*
 * VectorPtr.hpp
 *
 *  Created on: 23/6/2015
 *      Author: Daniel
 */

#include "VectorPtr.h"

template<typename T>
VectorPtr<T>::VectorPtr() {

}

template<typename T> template<typename T2>
VectorPtr<T>::VectorPtr(T2* elem) {
    element_.reserve(1);
    if (dynamic_cast<const T*>(elem) != NULL) {
        element_.push_back(
            std::dynamic_pointer_cast<T>(elem->getSharedPtr()));
    }
}

template<typename T> template<typename T2>
VectorPtr<T>::VectorPtr(const std::vector<T2*>& elems) {
    element_.reserve(elems.size());
    for (UInt i = 0; i < elems.size(); i++) {
        if (dynamic_cast<const T*>(elems[i]) != NULL) {
            element_.push_back(
                std::dynamic_pointer_cast<T>(elems[i]->getSharedPtr()));
        }
    }
}

template<typename T> template<typename T2>
VectorPtr<T>::VectorPtr(VectorPtr<T2>& rhs) {
    element_.reserve(rhs.size());
    for (UInt i = 0; i < rhs.size(); i++) {
        if (dynamic_cast<const T*>(rhs(i)) != NULL) {
            element_.push_back(
                std::dynamic_pointer_cast<T>(rhs(i)->getSharedPtr()));
        }
    }
}

template<typename T> template<typename T2>
VectorPtr<T>::VectorPtr(VectorPtr<T2>&& rhs) {
    element_.reserve(rhs.size());
    for (UInt i = 0; i < rhs.size(); i++) {
        if (dynamic_cast<const T*>(rhs(i)) != NULL) {
            element_.push_back(
                std::move(
                    std::dynamic_pointer_cast<T>(rhs(i)->getSharedPtr())));
        }
    }
}

template<typename T> template<typename T2>
VectorPtr<T>::VectorPtr(const VectorPtr<T2>& rhs) {
    static_assert(std::is_const<T>::value, "Template parameter must be const");
    element_.reserve(rhs.size());
    for (UInt i = 0; i < rhs.size(); i++) {
        if (dynamic_cast<const T*>(rhs(i)) != NULL) {
            element_.push_back(
                std::dynamic_pointer_cast<T>(rhs(i)->getSharedPtr()));
        }
    }
}

template<typename T>
VectorPtr<T>::VectorPtr(VectorPtr& rhs) {
    element_ = rhs.element_;
}

template<typename T>
VectorPtr<T>::VectorPtr(VectorPtr&& rhs) {
    element_ = std::move(rhs.element_);
}

template<typename T>
VectorPtr<T>::~VectorPtr() {

}

template<typename T>
VectorPtr<typename std::remove_const<T>::type>
        VectorPtr<T>::cloneElems() const {
    VectorPtr<typename std::remove_const<T>::type> res;
    res.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        res.add(this->get(i)->template cloneTo<
                                  typename std::remove_const<T>::type>());
    }
    return res;
}

template<typename T>
VectorPtr<T>& VectorPtr<T>::operator=(VectorPtr& rhs) {
    if (this == &rhs) {
        return *this;
    }
    clear();
    element_ = rhs.element_;
    return *this;
}

template<typename T>
VectorPtr<T>& VectorPtr<T>::operator=(VectorPtr&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    clear();
    element_ = std::move(rhs.element_);
    return *this;
}

template<typename T>
typename Reference<T*>::type VectorPtr<T>::get(const UInt i) {
    return element_[i].get();
}

template<typename T>
typename ConstReference<T*>::type VectorPtr<T>::get(const UInt i) const {
    return element_[i].get();
}

template<typename T>
VectorPtr<T> VectorPtr<T>::get(const std::vector<UInt>& pos_) {
    std::set<UInt> pos(pos_.begin(), pos_.end());
    VectorPtr<T> res;
    res.reserve(this->size());
    for (std::set<UInt>::const_iterator
         it = pos.begin(); it != pos.end(); ++it) {
        res.add(element_[*it].get());
    }
    return res;
}

template<typename T>
VectorPtr<const T> VectorPtr<T>::get(const std::vector<UInt>& pos_) const {
    std::set<UInt> pos(pos_.begin(), pos_.end());
    VectorPtr<T> res;
    res.reserve(this->size());
    for (std::set<UInt>::const_iterator
         it = pos.begin(); it != pos.end(); ++it) {
        res.add(element_[*it].get());
    }
    return res;
}

template<typename T>
void VectorPtr<T>::set(const UInt i, T* elem) {
    element_[i] = std::dynamic_pointer_cast<T>(elem->getSharedPtr());
}

template<typename T> template<typename T2>
VectorPtr<T> VectorPtr<T>::add(T2* elem) {
    return add(VectorPtr(elem));
}

template<typename T> template<typename T2>
VectorPtr<T> VectorPtr<T>::add(const std::vector<T2*>& elems) {
    return add(VectorPtr(elems));
}

template<typename T> template<typename T2>
VectorPtr<T> VectorPtr<T>::add(VectorPtr<T2>& rhs) {
    return add(VectorPtr<T>(rhs));
}

template<typename T> template<typename T2, typename, typename>
VectorPtr<T> VectorPtr<T>::add(const VectorPtr<T2>& rhs) {
    return add(VectorPtr<T>(rhs));
}

template<typename T>
VectorPtr<T> VectorPtr<T>::add(VectorPtr& rhs) {
    element_.insert(element_.end(), rhs.element_.begin(), rhs.element_.end());
    return rhs;
}

template<typename T>
VectorPtr<T> VectorPtr<T>::add(VectorPtr&& rhs) {
    element_.insert(element_.end(), rhs.element_.begin(), rhs.element_.end());
    return rhs;
}

template<typename T>
void VectorPtr<T>::remove(const UInt& pos) {
    std::vector<UInt> aux;
    aux.push_back(pos);
    remove(aux);
}

template<typename T>
void VectorPtr<T>::remove(const std::vector<UInt>& pos_) {
    std::set<UInt> pos(pos_.begin(), pos_.end());
    VectorPtr<T> aux;
    aux.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        if (!pos.count(i)) {
            aux.add(element_[i].get());
        }
    }
    operator=(std::move(aux));
}
