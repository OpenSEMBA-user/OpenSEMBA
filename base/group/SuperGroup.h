/*
 * SuperGroup.h
 *
 *  Created on: 8/4/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_BASE_GROUP_SUPERGROUP_H_
#define SRC_COMMON_BASE_GROUP_SUPERGROUP_H_

#include <map>
#include <memory>
using namespace std;

#include "base/class/ClassBase.h"

class SuperGroup {
    template<typename T>
    friend class Group;
    static map<const ClassBase*,shared_ptr<ClassBase>> elems;
};

#endif /* SRC_COMMON_BASE_GROUP_SUPERGROUP_H_ */
