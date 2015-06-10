/*
 * GroupIterator.hpp
 *
 *  Created on: 23/4/2015
 *      Author: Daniel
 */

#include "GroupIterator.h"

template<typename T>
GroupIterator<T>::GroupIterator() {
    group_ = NULL;
    pos_ = 0;
}

template<typename T>
GroupIterator<T>::GroupIterator(Group<T>& group, UInt pos) {
    group_ = &group;
    pos_ = pos;
}

template<typename T>
GroupIterator<T>::GroupIterator(const GroupIterator<T>& rhs) {
    group_ = rhs.group_;
    pos_ = rhs.pos_;
}

template<typename T>
GroupIterator<T>::~GroupIterator() {

}

template<typename T>
GroupIterator<T>& GroupIterator<T>::operator=(const GroupIterator<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    group_ = rhs.group_;
    pos_ = rhs.pos_;
    return *this;
}

template<typename T>
bool GroupIterator<T>::operator!=(const GroupIterator<T>& rhs) {
    return ((group_ != rhs.group_) || (pos_ != rhs.pos_));
}

template<typename T>
T&  GroupIterator<T>::operator*() {
    return *group_->get(pos_);
}

template<typename T>
T&  GroupIterator<T>::operator->() {
    return *group_->get(pos_);
}

template<typename T>
const T&  GroupIterator<T>::operator*() const {
    return *group_->get(pos_);
}

template<typename T>
const T&  GroupIterator<T>::operator->() const {
    return *group_->get(pos_);
}

template<typename T>
GroupIterator<T>& GroupIterator<T>::operator++() {
    ++pos_;
    return *this;
}
