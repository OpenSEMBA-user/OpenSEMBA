#ifndef GROUPBASE_H_
#define GROUPBASE_H_

#include "ClassBase.h"

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

    virtual void add(const T* newElem);
    virtual void add(const vector<T*>&);

protected:
    vector<T*> element_;
};

#include "GroupBase.hpp"

#endif /* GROUPBASE_H_ */
