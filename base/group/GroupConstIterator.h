/*
 * GroupConstIterator.h
 *
 *  Created on: 23/4/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_GROUP_GROUPCONSTITERATOR_H_
#define COMMON_BASE_GROUP_GROUPCONSTITERATOR_H_

#include <cstdlib>
using namespace std;

#include "GroupIterator.h"

template<typename T>
class GroupConstIterator {
public:
    GroupConstIterator();
    GroupConstIterator(const Group<T>& group, UInt pos);
    GroupConstIterator(const GroupIterator<T>& rhs);
    GroupConstIterator(const GroupConstIterator<T>& rhs);
    virtual ~GroupConstIterator();

    GroupConstIterator<T>& operator=(const GroupIterator<T>& rhs);
    GroupConstIterator<T>& operator=(const GroupConstIterator<T>& rhs);

    bool operator!=(const GroupConstIterator<T>& rhs);

    const T& operator* () const;
    const T& operator->() const;

    GroupConstIterator<T>& operator++();

private:
    const Group<T>* group_;
    UInt pos_;
};

#endif /* COMMON_BASE_GROUP_GROUPCONSTITERATOR_H_ */
