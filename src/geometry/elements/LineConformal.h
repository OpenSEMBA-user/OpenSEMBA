/*
 * Lin2Conformal.h
 *
 *  Created on: 21/3/2015
 *      Author: Daniel
 */

#ifndef SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_
#define SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_

#include <geometry/elements/Line2.h>
#include "geometry/coordinates/CoordinateConformal.h"


class LineConformal : public virtual Line2<Int> {
public:
    class ErrorCoordNotConf : public Element<Int>::ErrorCoord {
    public:
        ErrorCoordNotConf(const CoordinateId& coordId);
        ~ErrorCoordNotConf() throw();
    };

    LineConformal();
    LineConformal(const ElementId id,
                  const CoordI3* v[2],
                  const CVecR3& norm,
                  const LayerId layerId = LayerId(0),
                  const MatId   matId   = MatId(0));
    LineConformal(const CoordI3* v[2],
                  const CVecR3& norm,
                  const LayerId layerId = LayerId(0),
                  const MatId   matId   = MatId(0));
    LineConformal(const LineConformal& rhs);
    virtual ~LineConformal();

    DEFINE_CLONE(LineConformal);

    CVecR3 getNorm () const { return norm_;  }

    const CoordConf* getConfV(const UInt i) const;

    void setV(const UInt i, const CoordI3* coord);

    ElemR* toUnstructured(const GroupCoordinates<CoordR3>&, const Grid3&) const;

    void printInfo() const;

private:
    void checkCoordinates();
    CVecR3 norm_;
};

typedef LineConformal LinConf;

#endif /* SRC_COMMON_GEOMETRY_ELEMENTS_LINECONFORMAL_H_ */
