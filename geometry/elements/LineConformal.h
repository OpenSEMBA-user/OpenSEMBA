/*
 * Lin2Conformal.h
 *
 *  Created on: 21/3/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_
#define SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_

#include "CoordinateConformal.h"

#include "Lin2.h"

class LineConformal : public virtual LinI2 {
public:
    LineConformal();
    LineConformal(const ElementId id,
                  const CoordI3* v[2],
                  const LayerId layerId = LayerId(0),
                  const MatId   matId   = MatId(0));
    LineConformal(const CoordI3* v[2]);
    LineConformal(const LineConformal& rhs);
    virtual ~LineConformal();

    ClassBase* clone() const;

    const CoordConf* getConfV(const UInt i) const;

    void setV(const UInt i, const CoordI3* coord);

    void printInfo() const;

private:
    void checkCoordinates();
};

typedef LineConformal LinConf;

#endif /* SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_ */
