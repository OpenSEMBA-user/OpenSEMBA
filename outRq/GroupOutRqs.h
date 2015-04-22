#ifndef COMMON_OUTRQ_GROUPOUTRQS_H_
#define COMMON_OUTRQ_GROUPOUTRQS_H_

#include "OutRq.h"
#include "OutRqFarField.h"

template<typename O = OutRqBase>
class GroupOutRqs : public virtual Group<O> {
public:
    USE_GROUP_CONSTRUCTS(GroupOutRqs, O);

    DEFINE_GROUP_CLONE(GroupOutRqs, O);

    USE_GROUP_ASSIGN(O);

    template<typename O2>
    O* add(O2* newElem);
    template<typename O2>
    vector<O*> add(vector<O2*>&);
    template<typename O2>
    vector<O*> add(Group<O2>&);

    void printInfo() const;
};

#include "GroupOutRqs.hpp"

#endif
