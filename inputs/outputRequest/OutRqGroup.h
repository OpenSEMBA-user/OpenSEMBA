#ifndef OUTPUTREQUESTGROUP_H_
#define OUTPUTREQUESTGROUP_H_

#include "OutRq.h"
#include "OutRqFarField.h"

template<typename O = OutRq>
class OutRqGroup : public Group<O> {
public:
    OutRqGroup();
    OutRqGroup(const vector<O*>&);
    OutRqGroup(const Group<O>& rhs);
    virtual ~OutRqGroup();

    OutRqGroup<O>& operator=(const Group<O>& rhs);

    void add(OutRq*);
    void printInfo() const;
};

#include "OutRqGroup.hpp"

#endif
