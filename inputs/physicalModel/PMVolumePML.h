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
	typedef enum {
		PMLx,
		PMLy,
		PMLz,
		PMLxy,
		PMLyz,
		PMLzx,
		PMLxyz,
		undefined
	} Orientation;
	static const unsigned int possibleDirections = 26;
	PMVolumePML();
	PMVolumePML(
	 const unsigned int id,
	 const string& name_);
	PMVolumePML(
	 const unsigned int id,
	 const Direction direction[3],
	 const BoxD3& bound);
	~PMVolumePML();
	PMVolumePML&
	 operator=(const PMVolumePML& rhs);
	bool
	 isPML() const;
	void
	 printInfo() const;
	const Direction*
	 getDirection() const;
	Orientation
	 getOrientation() const;
	bool
	 isUniaxial() const;
	bool
	 isBiaxial() const;
	bool
	 isTriaxial() const;
	const pair<CVecD3, CVecD3>&
 	 getBound() const;
	CVecD3
	 getWidth() const;
	unsigned int
	 getFirstOrientationIndex() const;
	CVecD3
	 getPMLBeginningPosition() const;
	virtual bool
	 isClassic() const {return false;}
private:
	Direction direction[3];
	pair<CVecD3,CVecD3> bound;
	void
	 check() const;
};

#endif /* PMVOLUMEPML_H_ */
