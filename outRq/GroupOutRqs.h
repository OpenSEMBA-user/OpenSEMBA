#ifndef COMMON_OUTRQ_GROUPOUTRQS_H_
#define COMMON_OUTRQ_GROUPOUTRQS_H_

#include "OutRq.h"
#include "OutRqBulkCurrent.h"
#include "OutRqFarField.h"

template<typename O = OutRqBase>
class GroupOutRqs : public Group<O> {
public:
    GroupOutRqs() {}
    template<typename O2>
    GroupOutRqs(O2* e)                     : Group<O>(e) {}
    template<typename O2>
    GroupOutRqs(const std::vector<O2*>& e) : Group<O>(e) {}
    template<typename O2>
    GroupOutRqs(VectorPtr<O2>&       rhs) : Group<O>(rhs) {}
    template<typename O2>
    GroupOutRqs(const VectorPtr<O2>& rhs) : Group<O>(rhs) {}
    GroupOutRqs(VectorPtr<O>&        rhs) : Group<O>(rhs) {}
    template<typename O2>
    GroupOutRqs(VectorPtr<O2>&& rhs) : Group<O>(std::move(rhs)) {}
    GroupOutRqs(VectorPtr<O >&& rhs) : Group<O>(std::move(rhs)) {}
    virtual ~GroupOutRqs() {}

    DEFINE_GROUP_CLONE(GroupOutRqs, O);

    GroupOutRqs& operator=(VectorPtr<O>&);
    GroupOutRqs& operator=(VectorPtr<O>&&);

    template<typename O2>
    VectorPtr<O> add(O2* newElem);
    template<typename O2>
    vector<O*> add(vector<O2*>&);
    template<typename O2>
    vector<O*> add(Group<O2>&);

    bool isSimilar(const GroupOutRqs& rhs) const;

    void printInfo() const;
};

#include "GroupOutRqs.hpp"

#endif
