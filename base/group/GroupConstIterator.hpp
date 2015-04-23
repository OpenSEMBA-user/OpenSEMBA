/*
 * GroupConstIterator.hpp
 *
 *  Created on: 23/4/2015
 *      Author: Daniel
 */

#include "GroupConstIterator.h"

template<typename T>
GroupConstIterator<T>::GroupConstIterator() {
    group_ = NULL;
    pos_ = 0;
}

template<typename T>
GroupConstIterator<T>::GroupConstIterator(const Group<T>& group, UInt pos) {
    group_ = &group;
    pos_ = pos;
}

template<typename T>
GroupConstIterator<T>::GroupConstIterator(const GroupIterator<T>& rhs) {
    group_ = rhs.group_;
    pos_ = rhs.pos_;
}

template<typename T>
GroupConstIterator<T>::GroupConstIterator(const GroupConstIterator<T>& rhs) {
    group_ = rhs.group_;
    pos_ = rhs.pos_;
}

template<typename T>
GroupConstIterator<T>::~GroupConstIterator() {

}

template<typename T>
GroupConstIterator<T>& GroupConstIterator<T>::operator=(
        const GroupIterator<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    group_ = rhs.group_;
    pos_ = rhs.pos_;
    return *this;
}

template<typename T>
GroupConstIterator<T>& GroupConstIterator<T>::operator=(
        const GroupConstIterator<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    group_ = rhs.group_;
    pos_ = rhs.pos_;
    return *this;
}

template<typename T>
bool GroupConstIterator<T>::operator!=(const GroupConstIterator<T>& rhs) {
    return ((group_ != rhs.group_) || (pos_ != rhs.pos_));
}

template<typename T>
const T&  GroupConstIterator<T>::operator*() const {
    return *group_->get(pos_);
}

template<typename T>
const T&  GroupConstIterator<T>::operator->() const {
    return *group_->get(pos_);
}

template<typename T>
GroupConstIterator<T>& GroupConstIterator<T>::operator++() {
    ++pos_;
    return *this;
}
