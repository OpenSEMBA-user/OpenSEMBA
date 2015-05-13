/*
 * ResumeOutput.h
 *
 *  Created on: Aug 23, 2012
 *      Author: luis
 */

#ifndef OUTPUTRESUME_H_
#define OUTPUTRESUME_H_

#include "../../../common/exporter/Output.h"

class OutputResume: public Exporter {
public:
	OutputResume();
	virtual ~OutputResume();
	void
	 writeResumeResFile(
	  const double *Ex, const double *Ey, const double *Ez,
	  const double *Hx, const double *Hy, const double *Hz,
	  const int tStep, const uint numElems) const;
};

#endif /* RESUMEOUTPUT_H_ */
