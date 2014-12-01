/*
 * Waveport.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef WAVEPORT_H_
#define WAVEPORT_H_

#include "EMSource.h"

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
	 const vector<unsigned int>& elem_,
	 const bool input_,
	 const double spread_,
	 const double delay_,
	 const Shape shape_,
	 const ExcitationMode excMode_,
	 const pair<unsigned int,unsigned int> mode_,
	 const Symmetry xy_,
	 const Symmetry yz_,
	 const Symmetry zx_);
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
	bool input;
	Shape shape;
	ExcitationMode excitationMode;
	pair<unsigned int,unsigned int> mode;
	Symmetry SymXY, SymYZ, SymZX;
};

#endif /* WAVEPORT_H_ */
