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
