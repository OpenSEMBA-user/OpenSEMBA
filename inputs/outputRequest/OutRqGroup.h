#ifndef OUTPUTREQUESTGROUP_H_
#define OUTPUTREQUESTGROUP_H_

#include "OutRq.h"
#include "OutRqFarField.h"

template<typename C = OutRq>
class OutRqGroup : public Group<OutRq> {
public:
    OutRqGroup();
	OutRqGroup(const vector<OutRq>&);
	UInt countWithType(const Element::Type) const;
	const OutRq* getWithType(const UInt i, const Element::Type type) const;
	void printInfo() const;
};

#include "OutRqGroup.hpp"

#endif
