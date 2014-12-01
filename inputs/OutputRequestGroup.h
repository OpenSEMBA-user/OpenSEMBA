#ifndef OUTPUTREQUESTGROUP_H_
#define OUTPUTREQUESTGROUP_H_

#include "OutputRequest.h"

class OutputRequestGroup {
public:
	OutputRequestGroup();
	OutputRequestGroup(const vector<OutputRequest>&);
	void
	 applyGeometricScalingFactor(const double scalingFactor);
	uint
	 countWithType(const Element::Type) const;
	const OutputRequest*
	 getWithType(const uint i, const Element::Type type) const;
	void
	 printInfo() const;
//private:
	vector<OutputRequest> outputRequest;
};

#	endif
