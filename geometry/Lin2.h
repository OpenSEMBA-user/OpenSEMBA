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
	 const unsigned int id_,
	 const unsigned int matId_,
	 const unsigned int vId[2]);
	Lin2(
	 const unsigned int id_,
	 const unsigned int matId_,
	 const Coordinate<double,3>* v_[2]);
	virtual ~Lin2();
	const Coordinate<double,3>*
	 getV(const unsigned int i) const {return v[i];}
	const Coordinate<double,3>*
	 getSideV(const unsigned int face, const unsigned int i) const {
		return v[i];
	 }
	const Coordinate<double,3>*
	 getSideVertex(const unsigned int face, const unsigned int i) const {
		return v[i];
	 }
	void
	 setV(
	  const unsigned int i,
	  const Coordinate<double, 3>* constCoordinate);
	inline bool
	 isCurved() const {return false;}
	inline unsigned int
	 numberOfCoordinates() const {return 2;}
	const Coordinate<double,3>*
	 getVertex(const unsigned int i) const;
	void
	 printInfo() const;
private:
	static const SimplexLin<1> lin;
	const Coordinate<double,3>* v[2];
};
const SimplexLin<1> Lin2::lin;

#endif /* LIN2_H_ */
