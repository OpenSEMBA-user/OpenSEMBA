#ifndef GROUP_H_
#define GROUP_H_

#include <cassert>
#include <cstdlib>
#include <vector>

using namespace std;

#include "Types.h"

#include "ClassBase.h"

template<typename T>
class Group : public virtual ClassBase {
public:
    Group();
    Group(const vector<T*>& , bool ownership = true);
    Group(const Group<T>& rhs);
    virtual ~Group();

    ClassBase* clone() const;

    Group<T>& operator=(const Group<T>& rhs);

    bool empty() const { return element_.empty(); }
    UInt size () const { return element_.size();  }

    template<class T2>
    UInt sizeOf() const;

//    T*       operator()(const UInt i);
    const T* operator()(const UInt i) const;

    template<class T2>
    Group<T2> newGroupOf() const;
    
    template<class T2>
    Group<T2> getGroupOf() const;

    virtual void reserve(const UInt nS);

    virtual void add(T* newElem);
    virtual void add(vector<T*>&);
    virtual void add(const Group<T>&);

    void printInfo() const;

protected:
    bool ownership_;
    vector<T*> element_;

    void initElements(const vector<T*>& elems);
    void cloneElements(const vector<T*>& elems);
    void deleteElements();
};

#include "Group.hpp"

#endif /* GROUP_H_ */
