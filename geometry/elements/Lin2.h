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
         const MatId   matId   = MatId(0));
	Lin2(const ElementId id,
         const CoordR3* v[2],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Lin2(const Lin2& rhs);
	virtual ~Lin2();
    
    ClassBase* clone() const;
    
    inline UInt numberOfCoordinates() const { return 2; }
    
	const CoordR3* getV    (const UInt i) const { return v_[i]; }
	const CoordR3* getSideV(const UInt f,
                            const UInt i) const;
    
    const CoordR3* getVertex    (const UInt i) const;
	const CoordR3* getSideVertex(const UInt f,
                                 const UInt i) const;
	
    void setV(const UInt i, const CoordR3* coord);
	
	void printInfo() const;
private:
	static const SimplexLin<1> lin;
	const CoordR3* v_[2];
};
const SimplexLin<1> Lin2::lin;

#endif /* LIN2_H_ */
