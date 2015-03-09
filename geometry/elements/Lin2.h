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
	Lin2(const CoordinateGroup<>&,
         const ElementId id,
         const CoordinateId vId[2],
         const LayerId layerId = LayerId(0),
         const uint matId = 0);
	Lin2(const ElementId id,
         const CoordD3* v[2],
         const LayerId layerId = LayerId(0),
         const uint matId = 0);
    Lin2(const Lin2& rhs);
    Lin2(const ElementId id, const Lin2& rhs);
	virtual ~Lin2();
    
    ClassBase* clone()                   const;
    ClassBase* clone(const ElementId id) const;
    
    inline uint numberOfCoordinates() const { return 2; }
    
	const CoordD3* getV    (const uint i) const { return v_[i]; }
	const CoordD3* getSideV(const uint f,
                            const uint i) const;
    
    const CoordD3* getVertex    (const uint i) const;
	const CoordD3* getSideVertex(const uint f,
                                 const uint i) const;
	
    void setV(const uint i, const CoordD3* coord);
	
	void printInfo() const;
private:
	static const SimplexLin<1> lin;
	const CoordD3* v_[2];
};
const SimplexLin<1> Lin2::lin;

#endif /* LIN2_H_ */
