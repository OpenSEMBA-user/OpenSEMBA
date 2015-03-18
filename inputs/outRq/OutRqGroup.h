#ifndef OUTPUTREQUESTGROUP_H_
#define OUTPUTREQUESTGROUP_H_

#include "../outRq/OutRq.h"
#include "../outRq/OutRqPoint.h"
#include "../outRq/OutRqBulkCurrent.h"
#include "../outRq/OutRqVolume.h"
#include "../outRq/OutRqFarField.h"

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

#include "../outRq/OutRqGroup.hpp"

#endif
