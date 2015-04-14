#ifndef OUTPUTREQUESTGROUP_H_
#define OUTPUTREQUESTGROUP_H_

#include "OutRq.h"
#include "OutRqFarField.h"

template<typename O = OutRqBase>
class OutRqGroup : public virtual Group<O> {
public:
    USE_GROUP_CONSTRUCTS(OutRqGroup, O);

    USE_GROUP_ASSIGN(O);

    template<typename O2>
    O* add(O2* newElem);
    template<typename O2>
    vector<O*> add(vector<O2*>&);
    template<typename O2>
    vector<O*> add(Group<O2>&);

    void printInfo() const;
};

#include "../outRq/OutRqGroup.hpp"

#endif
