/*
 * SequenceConstIterator.hpp
 *
 *  Created on: 23/4/2015
 *      Author: Daniel
 */

#include "SequenceConstIterator.h"

template<typename T>
SequenceConstIterator<T>::SequenceConstIterator() {
    seq_ = NULL;
    pos_ = 0;
}

template<typename T>
SequenceConstIterator<T>::SequenceConstIterator(const Sequence<T>& seq,
                                                UInt pos) {
    seq_ = &seq;
    pos_ = pos;
}

template<typename T>
SequenceConstIterator<T>::SequenceConstIterator(
        const SequenceIterator<T>& rhs) {
    seq_ = rhs.group_;
    pos_ = rhs.pos_;
}

template<typename T>
SequenceConstIterator<T>::SequenceConstIterator(
        const SequenceConstIterator<T>& rhs) {
    seq_ = rhs.seq_;
    pos_ = rhs.pos_;
}

template<typename T>
SequenceConstIterator<T>::~SequenceConstIterator() {

}

template<typename T>
SequenceConstIterator<T>& SequenceConstIterator<T>::operator=(
        const SequenceIterator<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    seq_ = rhs.group_;
    pos_ = rhs.pos_;
    return *this;
}

template<typename T>
SequenceConstIterator<T>& SequenceConstIterator<T>::operator=(
        const SequenceConstIterator<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    seq_ = rhs.seq_;
    pos_ = rhs.pos_;
    return *this;
}

template<typename T>
bool SequenceConstIterator<T>::operator!=(const SequenceConstIterator<T>& rhs) {
    return ((seq_ != rhs.seq_) || (pos_ != rhs.pos_));
}

template<typename T>
ConstReference<T> SequenceConstIterator<T>::operator*() const {
    return seq_->get(pos_);
}

template<typename T>
ConstReference<T> SequenceConstIterator<T>::operator->() const {
    return seq_->get(pos_);
}

template<typename T>
SequenceConstIterator<T>& SequenceConstIterator<T>::operator++() {
    ++pos_;
    return *this;
}
