/*
 * Waveport.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef WAVEPORT_H_
#define WAVEPORT_H_

#include "EMSource.h"
#include "MagnitudeGaussian.h"
#include "MagnitudeNumerical.h"

class Waveport : public EMSource {
public:
	typedef enum {
		undefined,
		rectangular
	} Shape;
	typedef enum {
		TE,
		TM
	} ExcitationMode;
	typedef enum {
		none,
		pec,
		pmc
	} Symmetry;
	Waveport(
	        const vector<unsigned int>& elem,
	        const MagnitudeGaussian* magnitude,
	        const bool input,
	        const Shape shape,
	        const ExcitationMode excMode,
	        const pair<unsigned int,unsigned int> mode,
	        const Symmetry xy,
	        const Symmetry yz,
	        const Symmetry zx);
	virtual ~Waveport();
	Waveport&
	 operator=(const Waveport &rhs);
	void
	 printInfo() const;
	Shape
	 getShape() const;
	Symmetry
	 getSymXY() const;
	Symmetry
	 getSymYZ() const;
	Symmetry
	 getSymZX() const;
	ExcitationMode
	 getExcitationMode() const;
	pair<unsigned int, unsigned int>
	 getMode() const;
private:
	bool input_;
	Shape shape_;
	ExcitationMode excitationMode_;
	pair<unsigned int,unsigned int> mode_;
	Symmetry SymXY_, SymYZ_, SymZX_;
};

#endif /* WAVEPORT_H_ */
