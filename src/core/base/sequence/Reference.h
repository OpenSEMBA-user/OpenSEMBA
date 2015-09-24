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
 * Reference.h
 *
 *  Created on: 2/7/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_SEQUENCE_REFERENCE_H_
#define COMMON_BASE_SEQUENCE_REFERENCE_H_

#include <type_traits>

template<class T> struct IsConst                   : public std::false_type {};
template<class T> struct IsConst<const T>          : public std::true_type  {};
template<class T> struct IsConst<T&>               : public IsConst<T>      {};
template<class T> struct IsConst<T&&>              : public IsConst<T>      {};
template<class T> struct IsConst<T*>               : public IsConst<T>      {};
template<class T> struct IsConst<T* const>         : public IsConst<T>      {};
template<class T> struct IsConst<T* volatile>      : public IsConst<T>      {};
template<class T> struct IsConst<T* const volatile>: public IsConst<T>      {};

template <typename T>
struct AddConst {
    typedef typename std::add_const<T>::type type;
};
template <typename T>
struct AddConst<T&> {
    typedef typename AddConst<T>::type type;
};
template <typename T>
struct AddConst<T&&> {
    typedef typename AddConst<T>::type type;
};
template <typename T>
struct AddConst<T*> {
    typedef typename std::add_const<
        typename std::add_pointer<
            typename AddConst<T>::type>::type>::type type;
};
template <typename T>
struct AddConst<T* const> {
    typedef typename std::add_const<
        typename std::add_pointer<
            typename AddConst<T>::type>::type>::type type;
};
template <typename T>
struct AddConst<T* volatile> {
    typedef typename std::add_cv<
        typename std::add_pointer<
            typename AddConst<T>::type>::type>::type type;
};
template <typename T>
struct AddConst<T* const volatile> {
    typedef typename std::add_cv<
        typename std::add_pointer<
            typename AddConst<T>::type>::type>::type type;
};

template <typename T>
struct ConstReference {
    typedef typename std::conditional<
        std::is_pointer<T>::value,
        typename AddConst<T>::type,
        typename std::add_lvalue_reference<
            typename AddConst<T>::type>::type>::type type;
};

template <typename T>
struct Reference {
    typedef typename std::conditional<
        IsConst<T>::value,
        typename ConstReference<T>::type,
        typename std::conditional<
            std::is_pointer<T>::value, T, T&>::type>::type type;
};

#endif /* COMMON_BASE_SEQUENCE_REFERENCE_H_ */
