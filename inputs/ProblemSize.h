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
	ProblemSize() {
		v = 0;
		lin2 = 0;
		lin3 = 0;
		tri3 = 0;
		tri6 = 0;
		tet4 = 0;
		tet10 = 0;
		hex8 = 0;
		mat = 0;
	}
	ProblemSize
	 operator=(const ProblemSize& rhs) {
		if (&rhs == this) {
			return *this;
		}
		v = rhs.v;
		lin2 = rhs.lin2;
		lin3 = rhs.lin3;
		tri3 = rhs.tri3;
		tri6 = rhs.tri6;
		tet4 = rhs.tet4;
		tet10 = rhs.tet10;
		hex8 = rhs.hex8;
		mat = rhs.mat;
		return *this;
	}
	void
     printInfo() const {
		cout << " --- Problem size --- " << endl;
		cout << "Number of coordinates:  " << v << endl;
		cout << "Number of materials:    " << mat << endl;
		cout << "Number of hex8:         " << hex8 << endl;
		cout << "Number of tet10:        " << tet10 << endl;
		cout << "Number of tet4:         " << tet4 << endl;
		cout << "Number of tri6:         " << tri6 << endl;
		cout << "Number of tri3:         " << tri3 << endl;
		cout << "Number of lin3:         " << lin3 << endl;
		cout << "Number of lin2:         " << lin2 << endl;
		cout << " --- End of Problem size --- " << endl;
	}
};

#endif /* PROBLEMSIZE_H_ */
