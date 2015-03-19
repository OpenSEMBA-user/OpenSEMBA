#ifndef GROUPID_H_
#define GROUPID_H_

#include <iostream>
#include <map>

using namespace std;

#include "Group.h"

template<typename T, class Id>
class GroupId : public Group<T> {
public:
    GroupId();
    GroupId(const vector<T*>&, bool ownership = true);
    GroupId(const Group<T>& rhs);
    virtual ~GroupId();

    GroupId<T, Id>& operator=(const Group<T>& rhs);

    bool existId(const Id id) const;

//    T*       getPtrToId(const Id id);
    const T* getPtrToId(const Id id) const;

    GroupId<T, Id> get(const vector<Id>&) const;

    void add(T* newElem , bool newId = false);
    void add(vector<T*>&, bool newId = false);
    void add(const Group<T>&);

protected:
    Id lastId_;
    map<Id, UInt> mapId_;

    void buildMapId();
};

#include "GroupId.hpp"

#endif /* GROUPID_H_ */
