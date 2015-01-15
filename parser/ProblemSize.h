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
//typedef unsigned int uint;;

class ProblemSize {
public:
	unsigned int v;	    // Number of coordinate points.
	unsigned int lin2;  // # linear segment lines.
	unsigned int lin3;  // # quadratic segment lines.
	unsigned int tri3;  // # triangular elements.
	unsigned int tri6;  // #
	unsigned int tet4;  // # tetrahedron elements.
	unsigned int tet10; // #
	unsigned int hex8;
	unsigned int mat;   // # materials.
	ProblemSize();
	ProblemSize
	 operator=(const ProblemSize& rhs);
	void
     printInfo() const;
};

#endif /* PROBLEMSIZE_H_ */
