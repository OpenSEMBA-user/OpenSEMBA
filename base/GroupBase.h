#ifndef GROUPBASE_H_
#define GROUPBASE_H_

template<typename T>
class GroupBase {
public:
    GroupBase();
    GroupBase(const vector<T*>&);
    GroupBase(const GroupBase<T>& rhs);
    virtual ~GroupBase();

    GroupBase<T>& operator=(const GroupBase<T>& rhs);

    unsigned size() const { return element_.size(); }

    const T* operator()(const unsigned i) const;

    template<class T2>
    GroupBase<T2> getGroupOf() const;
    
    template<class T2>
    vector<const T2*> getVectorOf() const;
    
    template<class T2>
    unsigned numberOf() const;

    virtual const T*         add(T* newElem);
    virtual vector<const T*> add(const vector<T*>&);

protected:
    vector<T*> element_;

    void cloneElements(const vector<T*>& elems);
    void deleteElements();
};

#include "GroupBase.hpp"

#endif /* GROUPBASE_H_ */
