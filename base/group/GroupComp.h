/*
 * GroupGroup.h
 *
 *  Created on: 5/4/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_BASE_GROUPCOMP_H_
#define SRC_COMMON_BASE_GROUPCOMP_H_

#include <typeinfo>

using namespace std;

#include "Group.h"

template<typename T>
class GroupComp : public virtual Group<T> {
public:
    USE_GROUP_CONSTRUCTS(GroupComp, T);

    DEFINE_GROUP_CLONE(GroupComp, T);

    USE_GROUP_ASSIGN(T);

    template<class T2>
    GroupComp<typename conditional<is_const<T>::value, const T2, T2>::type>
        getGroupOfOnly();
    template<class T2>
    GroupComp<const T2> getGroupOfOnly() const;
    template<class T2>
    GroupComp<T> getGroupNotOfOnly();
    template<class T2>
    GroupComp<const T> getGroupNotOfOnly() const;

private:
    template<class T2>
    vector<UInt> getElemsWith_() const;
};

#include "GroupComp.hpp"

#define USE_GROUPCOMP_GETGROUPOFONLY(T)                                     \
    template<class T2>                                                      \
    GroupComp<typename conditional<is_const<T>::value, const T2, T2>::type> \
        getGroupOf() {                                                      \
        return GroupComp<T>::getGroupOfOnly<T2>();                          \
    }                                                                       \
    template<class T2>                                                      \
    GroupComp<const T2> getGroupOfOnly() const {                            \
        return GroupComp<T>::getGroupOfOnly<T2>();                          \
    }                                                                       \
    template<class T2>                                                      \
    GroupComp<T> getGroupNotOfOnly() {                                      \
        return GroupComp<T>::getGroupNotOfOnly<T2>();                       \
    }                                                                       \
    template<class T2>                                                      \
    GroupComp<const T> getGroupNotOfOnly() const {                          \
        return GroupComp<T>::getGroupNotOfOnly<T2>();                       \
    }

#endif /* SRC_COMMON_BASE_GROUPCOMP_H_ */
