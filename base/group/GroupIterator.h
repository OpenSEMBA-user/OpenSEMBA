/*
 * GroupIterator.h
 *
 *  Created on: 23/4/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_GROUP_GROUPITERATOR_H_
#define COMMON_BASE_GROUP_GROUPITERATOR_H_

#include <cstdlib>
using namespace std;

#include "Types.h"

template<typename T>
class Group;

template<typename T>
class GroupIterator {
public:
    GroupIterator();
    GroupIterator(Group<T>& group, UInt pos);
    GroupIterator(const GroupIterator<T>& rhs);
    virtual ~GroupIterator();

    GroupIterator<T>& operator=(const GroupIterator<T>& rhs);

    bool operator!=(const GroupIterator<T>& rhs);

    T& operator* ();
    T& operator->();
    const T& operator* () const;
    const T& operator->() const;

    GroupIterator<T>& operator++();

private:
    Group<T>* group_;
    UInt pos_;
};

#endif /* COMMON_BASE_GROUP_GROUPITERATOR_H_ */
