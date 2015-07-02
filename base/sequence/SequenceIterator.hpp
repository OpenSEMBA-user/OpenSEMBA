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
