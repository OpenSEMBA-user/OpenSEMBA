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

#include "Identification.h"

#include <sstream>

namespace SEMBA {
namespace Class {

template<typename T>
Identification<T>::Identification()
:   id_(0) {

}

template<typename T>
Identification<T>::Identification(const std::size_t id)
:   id_(id) {

}

template<typename T>
Identification<T>::Identification(const Identification& rhs)
:   id_(rhs.id_) {

}

template<typename T>
Identification<T>::~Identification() {

}

template<typename T>
Identification<T>& Identification<T>::operator=(const Identification& rhs) {
    if(this == &rhs) {
        return *this;
    }
    id_ = rhs.id_;
    return *this;
}

template<typename T>
Identification<T>& Identification<T>::operator+=(const Identification& rhs) {
    id_ += rhs.id_;
    return *this;
}

template<typename T>
bool Identification<T>::operator==(const Identification& rhs) const {
    return id_ == rhs.id_;
}

template<typename T>
bool Identification<T>::operator!=(const Identification& rhs) const {
    return id_ != rhs.id_;
}

template<typename T>
bool Identification<T>::operator<(const Identification& rhs) const {
    return id_ < rhs.id_;
}

template<typename T>
bool Identification<T>::operator<=(const Identification& rhs) const {
    return id_ <= rhs.id_;
}

template<typename T>
bool Identification<T>::operator>(const Identification& rhs) const {
    return id_ > rhs.id_;
}

template<typename T>
bool Identification<T>::operator>=(const Identification& rhs) const {
    return id_ >= rhs.id_;
}

template<typename T>
Identification<T> Identification<T>::operator+(const Identification& rhs) const {
    return Identification(id_ + rhs.id_);
}

template<typename T>
Identification<T>& Identification<T>::operator++() {
    id_++;
    return *this;
}

template<typename T>
Identification<T> Identification<T>::operator++(int) {
    Identification copy(*this);
    id_++;
    return copy;
}

template<typename T>
std::size_t Identification<T>::toInt() const {
    return id_;
}

template<typename T>
std::string Identification<T>::toStr() const {
    std::stringstream aux;
    aux << id_;
    return aux.str();
}

template <typename I>
std::istream& operator>>(std::istream& input, Identification<I>& id) {
    input >> id.id_;
    return input;
}

template <typename I>
std::ostream& operator<<(std::ostream& output, const Identification<I>& id) {
    output << id.id_;
    return output;
}

} /* namespace Class */
} /* namespace SEMBA */
