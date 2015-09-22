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
 * Id.hpp
 *
 *  Created on: 22/6/2015
 *      Author: Daniel
 */

#include "Id.h"

template<typename T>
Id<T>::Id()
:   id_(0) {

}

template<typename T>
Id<T>::Id(const UInt id)
:   id_(id) {

}

template<typename T>
Id<T>::Id(const Id& rhs)
:   id_(rhs.id_) {

}

template<typename T>
Id<T>::~Id() {

}

template<typename T>
Id<T>& Id<T>::operator=(const Id& rhs) {
    if(this == &rhs) {
        return *this;
    }
    id_ = rhs.id_;
    return *this;
}

template<typename T>
Id<T>& Id<T>::operator+=(const Id& rhs) {
    id_ += rhs.id_;
    return *this;
}

template<typename T>
bool Id<T>::operator==(const Id& rhs) const {
    return id_ == rhs.id_;
}

template<typename T>
bool Id<T>::operator!=(const Id& rhs) const {
    return id_ != rhs.id_;
}

template<typename T>
bool Id<T>::operator<(const Id& rhs) const {
    return id_ < rhs.id_;
}

template<typename T>
bool Id<T>::operator<=(const Id& rhs) const {
    return id_ <= rhs.id_;
}

template<typename T>
bool Id<T>::operator>(const Id& rhs) const {
    return id_ > rhs.id_;
}

template<typename T>
bool Id<T>::operator>=(const Id& rhs) const {
    return id_ >= rhs.id_;
}

template<typename T>
Id<T> Id<T>::operator+(const Id& rhs) const {
    return Id(id_ + rhs.id_);
}

template<typename T>
Id<T>& Id<T>::operator++() {
    id_++;
    return *this;
}

template<typename T>
Id<T> Id<T>::operator++(int) {
    Id copy(*this);
    id_++;
    return copy;
}

template<typename T>
UInt Id<T>::toUInt() const {
    return id_;
}

template<typename T>
std::string Id<T>::toStr() const {
    std::stringstream aux;
    aux << id_;
    return aux.str();
}

template <typename I>
std::istream& operator>>(std::istream& input, Id<I>& id) {
    input >> id.id_;
    return input;
}

template <typename I>
std::ostream& operator<<(std::ostream& output, const Id<I>& id) {
    output << id.id_;
    return output;
}
