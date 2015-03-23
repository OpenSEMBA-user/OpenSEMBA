#ifndef OUTPUTREQUESTGROUP_H_
#define OUTPUTREQUESTGROUP_H_

#include "OutRq.h"
#include "OutRqFarField.h"

template<typename O = OutRq<> >
class OutRqGroup : public Group<O> {
public:
    OutRqGroup();
    template<typename O2>
    OutRqGroup(const vector<O2*>&);
    OutRqGroup(const Group<O>& rhs);
    template<typename O2>
    OutRqGroup(const Group<O2>& rhs);
    virtual ~OutRqGroup();

    OutRqGroup<O>& operator=(const Group<O>& rhs);
    template<typename O2>
    OutRqGroup<O>& operator=(const Group<O2>& rhs);

    template<typename T2>
    void add(T2* newElem);

    void printInfo() const;
};

#include "../outRq/OutRqGroup.hpp"

#endif
