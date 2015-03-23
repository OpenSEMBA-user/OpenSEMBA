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
    template<typename T2>
    GroupId(const vector<T2*>&);
    template<typename T2>
    GroupId(const vector<T2*>&, bool ownership);
    GroupId(const Group<T>& rhs);
    template<typename T2>
    GroupId(const Group<T2>& rhs);
    virtual ~GroupId();

    GroupId<T, Id>& operator=(const Group<T>& rhs);
    template<typename T2>
    GroupId<T, Id>& operator=(const Group<T2>& rhs);

    bool existId(const Id id) const;

//    T*       getPtrToId(const Id id);
    const T* getPtrToId(const Id id) const;

    GroupId<T, Id> get(const vector<Id>&) const;

    template<typename T2>
    void add(T2* newElem , bool newId = false);
    template<typename T2>
    void add(vector<T2*>&, bool newId = false);
    template<typename T2>
    void add(const Group<T2>&);

protected:
    Id lastId_;
    map<Id, UInt> mapId_;

private:
    void buildMapId();
};

#include "GroupId.hpp"

#endif /* GROUPID_H_ */
