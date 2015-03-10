#ifndef GROUP_H_
#define GROUP_H_

#include <cassert>
#include <cstdlib>
#include <vector>

using namespace std;

typedef unsigned int uint;

template<typename T>
class Group {
public:
    Group();
    Group(const vector<T*>& , bool ownership = true);
    Group(const Group<T>& rhs);
    virtual ~Group();

    Group<T>& operator=(const Group<T>& rhs);

    unsigned size() const { return element_.size(); }

    T*       operator()(const unsigned i);
    const T* operator()(const unsigned i) const;

    template<class T2>
    Group<T2> newGroupOf() const;
    
    template<class T2>
    Group<T2> getGroupOf() const;

    template<class T2>
    vector<const T2*> getVectorOf() const;
    
    template<class T2>
    unsigned sizeOf() const;

    virtual void add(T* newElem);
    virtual void add(vector<T*>&);

protected:
    bool ownership_;
    vector<T*> element_;

    void initElements(const vector<T*>& elems);
    void cloneElements(const vector<T*>& elems);
    void deleteElements();
};

#include "Group.hpp"

#endif /* GROUP_H_ */
