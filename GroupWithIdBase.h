#ifndef GROUPWITHIDBASE_H_
#define GROUPWITHIDBASE_H_

#include <iostream>

using namespace std;

#include "GroupBase.h"

template<typename T, class Id>
class GroupWithIdBase : public GroupBase<T, Id> {
public:
   GroupWithIdBase();
   GroupWithIdBase(const vector<T*>&);
   GroupWithIdBase(const GroupWithIdBase<T, Id>& rhs);
   virtual ~GroupWithIdBase();
   
   GroupWithIdBase<T, Id>& operator=(const GroupWithIdBase<T, Id>& rhs);
   
   const T* getPtrToId(const Id id) const;
   
   template<class T2>
   GroupWithIdBase<T2, Id> getGroupOf() const;
   
   virtual Id         add(const T* newElem);
   virtual vector<Id> add(const vector<T*>&);
   
protected:
   Id lastId_;
   map<Id, unsigned> mapId_;
};

#include "GroupWithIdBase.hpp"

#endif /* GROUPWITHIDBASE_H_ */
