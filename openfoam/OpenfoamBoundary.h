/*
 * OpenfoamBoundary.h
 *
 *  Created on: Apr 11, 2014
 *      Author: luis
 */

#ifndef OPENFOAMBOUNDARY_H_
#define OPENFOAMBOUNDARY_H_

#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <assert.h>

using namespace std;

typedef unsigned int uint;

class OpenfoamBoundary {
public:
	OpenfoamBoundary();
	OpenfoamBoundary(
	 const uint& id,
	 const string& name,
	 const uint& nFaces,
	 const uint& startFace);
	virtual ~OpenfoamBoundary();
	OpenfoamBoundary&
	 operator=(const OpenfoamBoundary& rhs);
	string
	 getName() const;
	uint
	 getFaces() const;
	uint
	 getStartFace() const;
	void
	 printInfo() const;
	bool
	 isMaterial() const;
	bool
	 isOutputRequest() const;
	uint
	 getMaterialId() const;
    uint
     getLayerId() const;
	uint
	 getOutputRequestId() const;
private:
	string name_;
	uint id_;
	uint nFaces_, startFace_;
	int
	 strpos(const char *haystack, char *needle, int nth) const;
};

#endif /* OPENFOAMBOUNDARY_H_ */
