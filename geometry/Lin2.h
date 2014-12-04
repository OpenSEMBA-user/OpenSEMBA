/*
 * Lin2.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef LIN2_H_
#define LIN2_H_

#include "Line.h"

class Lin2 : public Line {
public:
	Lin2();
	Lin2(
	 const CoordinateGroup&,
	 const uint vId[2],
	 const uint id_,
     const uint matId_,
     const uint layerId_);
	Lin2(
     const CoordD3* v_[2],
     const uint id_,
	 const uint matId_,
	 const uint layerId_);
	virtual ~Lin2();
	const CoordD3*
	 getV(const uint i) const {return v[i];}
	const CoordD3*
	 getSideV(const uint face, const uint i) const {
		return v[i];
	 }
	const CoordD3*
	 getSideVertex(const uint face, const uint i) const {
		return v[i];
	 }
	void
	 setV(
	  const uint i,
	  const Coordinate<double, 3>* constCoordinate);
	inline bool
	 isCurved() const {return false;}
	inline uint
	 numberOfCoordinates() const {return 2;}
	const CoordD3*
	 getVertex(const uint i) const;
	void
	 printInfo() const;
private:
	static const SimplexLin<1> lin;
	const CoordD3* v[2];
};
const SimplexLin<1> Lin2::lin;

#endif /* LIN2_H_ */
