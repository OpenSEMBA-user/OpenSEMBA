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
    template<typename T2>
    friend class Group;
public:
    Group();
    template<typename T2>
    Group(const vector<T2*>&);
    template<typename T2>
    Group(const vector<T2*>& , bool ownership);
    Group(const Group<T>& rhs);
    template<typename T2>
    Group(const Group<T2>& rhs);
    virtual ~Group();

    ClassBase* clone() const;

    Group<T>& operator=(const Group<T>& rhs);
    template<typename T2>
    Group<T>& operator=(const Group<T2>& rhs);

    bool empty() const { return element_.empty(); }
    UInt size () const { return element_.size();  }
    void clear();

    template<class T2>
    UInt sizeOf() const;

//    T*       operator()(const UInt i);
    const T* operator()(const UInt i) const;

    template<class T2>
    Group<T2> newGroupOf() const;
    
    template<class T2>
    Group<T2> getGroupOf() const;

    virtual void reserve(const UInt nS);

    template<typename T2>
    void add(T2* newElem);
    template<typename T2>
    void add(vector<T2*>&);
    template<typename T2>
    void add(const Group<T2>&);

    void printInfo() const;

protected:
    bool ownership_;
    vector<T*> element_;

private:
    template<typename T2>
    void initElements(const vector<T2*>& elems);
    template<typename T2>
    void cloneElements(const vector<T2*>& elems);
};

#include "Group.hpp"

#endif /* GROUP_H_ */
