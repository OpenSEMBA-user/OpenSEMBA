/*
 * VectorModuleResult.h
 *
 *  Created on: Aug 22, 2012
 *      Author: luis
 */

#ifndef VECTORMODULERESULT_H_
#define VECTORMODULERESULT_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include "../../../common/math/CartesianVector.h"
#include "../core/Cell.h"

#ifndef VECTORRESULT_ERROR
	#define VECTORRESULT_ERROR 730
#endif

class VectorModuleResult {
public:
	string xLabel, yLabel;
	double where; // where:= e.g, time step or time.
	string componentNames[4];
	vector<int> valuePos;
	vector<double> values[3];
	uint numberOfValues;
	VectorModuleResult(const uint numberOfValues_);
	~VectorModuleResult();
	VectorModuleResult&
	 operator=(VectorModuleResult& rhs);
	void
	 readResult(ifstream& f_in);
	void
	 printInfo() const;
	};
#endif /* VECTORMODULERESULT_H_ */
