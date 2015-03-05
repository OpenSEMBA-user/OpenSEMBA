#ifndef OUTPUTREQUESTGROUP_H_
#define OUTPUTREQUESTGROUP_H_

#include "OutputRequest.h"

class OutputRequestGroup : GroupBase<OutputRequest> {
public:
	OutputRequestGroup(const vector<OutputRequest>&);
	uint countWithType(const Element::Type) const;
	const OutputRequest* getWithType(const uint i, const Element::Type type) const;
	void printInfo() const;
};

#	endif
