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
 * SequenceIterator.hpp
 *
 *  Created on: 23/4/2015
 *      Author: Daniel
 */

#include "SequenceIterator.h"

template<typename T>
SequenceIterator<T>::SequenceIterator() {
    seq_ = NULL;
    pos_ = 0;
}

template<typename T>
SequenceIterator<T>::SequenceIterator(Sequence<T>& seq, UInt pos) {
    seq_ = &seq;
    pos_ = pos;
}

template<typename T>
SequenceIterator<T>::SequenceIterator(const SequenceIterator<T>& rhs) {
    seq_ = rhs.seq_;
    pos_ = rhs.pos_;
}

template<typename T>
SequenceIterator<T>::~SequenceIterator() {

}

template<typename T>
SequenceIterator<T>& SequenceIterator<T>::operator=(
        const SequenceIterator<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    seq_ = rhs.seq_;
    pos_ = rhs.pos_;
    return *this;
}

template<typename T>
bool SequenceIterator<T>::operator!=(const SequenceIterator<T>& rhs) {
    return ((seq_ != rhs.seq_) || (pos_ != rhs.pos_));
}

template<typename T>
Reference<T> SequenceIterator<T>::operator*() {
    return seq_->get(pos_);
}

template<typename T>
Reference<T> SequenceIterator<T>::operator->() {
    return seq_->get(pos_);
}

template<typename T>
ConstReference<T> SequenceIterator<T>::operator*() const {
    return seq_->get(pos_);
}

template<typename T>
ConstReference<T> SequenceIterator<T>::operator->() const {
    return seq_->get(pos_);
}

template<typename T>
SequenceIterator<T>& SequenceIterator<T>::operator++() {
    ++pos_;
    return *this;
}
