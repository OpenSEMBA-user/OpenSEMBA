/*
 * PMVolumePML.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMVOLUMEPML_H_
#define PMVOLUMEPML_H_

#include "PMVolume.h"


class PMVolumePML : public PMVolume {
public:
	typedef enum {
		plus = 0,
		minus = 1,
		none = 2
	} Direction;
	enum class Orientation {
		PMLx,
		PMLy,
		PMLz,
		PMLxy,
		PMLyz,
		PMLzx,
		PMLxyz,
		undefined
	} ;
	static const UInt possibleDirections = 26;
	PMVolumePML(const MatId id, const string& name_);
	PMVolumePML(
	 const MatId id,
	 const Direction direction[3],
	 const BoxR3& bound);
	~PMVolumePML();

	DEFINE_CLONE(PMVolumePML);

	const Direction* getDirection() const;
	Orientation getOrientation() const;
	bool isUniaxial() const;
	bool isBiaxial() const;
	bool isTriaxial() const;
	const pair<CVecR3, CVecR3>& getBound() const;
	CVecR3 getWidth() const;
	UInt getFirstOrientationIndex() const;
	CVecR3 getPMLBeginningPosition() const;
	void printInfo() const;
private:
	Direction direction[3];
	pair<CVecR3,CVecR3> bound;
};

#endif /* PMVOLUMEPML_H_ */
