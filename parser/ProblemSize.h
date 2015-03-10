/*
 * ProblemSize.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#ifndef PROBLEMSIZE_H_
#define PROBLEMSIZE_H_

#include <iostream>

using namespace std;

#include "Types.h"

class ProblemSize {
public:
	UInt v;	    // Number of coordinate points.
	UInt lin2;  // # linear segment lines.
	UInt lin3;  // # quadratic segment lines.
	UInt tri3;  // # triangular elements.
	UInt tri6;  // #
	UInt tet4;  // # tetrahedron elements.
	UInt tet10; // #
	UInt hex8;
	UInt mat;   // # materials.
	ProblemSize();
	ProblemSize
	 operator=(const ProblemSize& rhs);
	void
     printInfo() const;
};

#endif /* PROBLEMSIZE_H_ */
