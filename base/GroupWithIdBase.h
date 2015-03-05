#ifndef GROUPWITHIDBASE_H_
#define GROUPWITHIDBASE_H_

#include <iostream>

using namespace std;

#include "IdBase.h"

#include "GroupBase.h"

template<typename T, class Id>
class GroupWithIdBase : public GroupBase<T> {
public:
    GroupWithIdBase();
    GroupWithIdBase(const vector<T*>&);
    GroupWithIdBase(const GroupBase<T>& rhs);
    virtual ~GroupWithIdBase();

    GroupWithIdBase<T, Id>& operator=(const GroupBase<T>& rhs);

    const T* getPtrToId(const Id id) const;

    const T*          add(T* newElem);
    vector<const T* > add(const vector<T*>&);

protected:
    Id lastId_;
    map<Id, unsigned> mapId_;

    void buildMapId();
};

#include "GroupWithIdBase.hpp"

#endif /* GROUPWITHIDBASE_H_ */
