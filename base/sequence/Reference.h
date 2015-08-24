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
