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
	static const uint possibleDirections = 26;
	PMVolumePML(const uint id, const string& name_);
	PMVolumePML(
	 const uint id,
	 const Direction direction[3],
	 const BoxD3& bound);
	~PMVolumePML();
	bool isPML() const;
	void printInfo() const;
	const Direction* getDirection() const;
	Orientation getOrientation() const;
	bool isUniaxial() const;
	bool isBiaxial() const;
	bool isTriaxial() const;
	const pair<CVecD3, CVecD3>& getBound() const;
	CVecD3 getWidth() const;
	uint getFirstOrientationIndex() const;
	CVecD3 getPMLBeginningPosition() const;
	virtual bool isClassic() const {return false;}
private:
	Direction direction[3];
	pair<CVecD3,CVecD3> bound;
};

#endif /* PMVOLUMEPML_H_ */
