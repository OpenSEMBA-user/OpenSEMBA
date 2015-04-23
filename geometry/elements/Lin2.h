/*
 * Lin2.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef LIN2_H_
#define LIN2_H_

#include "Line.h"

class Line2Base : public virtual LineBase {
public:
    Line2Base() {};
    virtual ~Line2Base() {};

    inline UInt numberOfCoordinates() const { return 2; }
};

template<class T>
class Line2 : public virtual Line<T>,
              public virtual Line2Base {
public:
    Line2();
    Line2(const CoordinateGroup<Coordinate<T,3> >&,
          const ElementId id,
          const CoordinateId vId[2],
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
    Line2(const ElementId id,
          const Coordinate<T,3>* v[2],
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
    Line2(CoordinateGroup<Coordinate<T,3> >&,
          const ElementId id,
          const Box<T,3>& box,
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
    Line2(const CoordinateGroup<Coordinate<T,3> >&,
          const CoordinateId vId[2]);
    Line2(const Coordinate<T,3>* v[2]);
    Line2(CoordinateGroup<Coordinate<T,3> >&,
          const Box<T,3>& box);
    Line2(const Line2<T>& rhs);
    virtual ~Line2();
    
    DEFINE_CLONE(Line2<T>);

    bool isStructured(const Grid3&, const Real = Grid3::tolerance) const;

    const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideV(const UInt f,
                               const UInt i) const;

    const Coordinate<T,3>* getVertex    (const UInt i) const;
    const Coordinate<T,3>* getSideVertex(const UInt f,
                                         const UInt i) const;

    vector<const Coordinate<T,3>*> getVertices() const;
    vector<const Coordinate<T,3>*> getSideVertices(const UInt face) const;

    void setV(const UInt i, const Coordinate<T,3>* coord);

    ElemI* toStructured(const CoordinateGroup<CoordI3>&,
                        const Grid3&, const Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const CoordinateGroup<CoordR3>&, const Grid3&) const;

    void printInfo() const;

private:
    static const SimplexLin<1> lin;
    const Coordinate<T,3>* v_[2];

    void setCoordinates(const CoordinateGroup<Coordinate<T,3> >&,
                        const CoordinateId vId[2]);
    void setCoordinates(const Coordinate<T,3>* v[2]);
    void setCoordinates(CoordinateGroup<Coordinate<T,3> >&,
                        const Box<T,3>& box);
};

typedef Line2Base   Lin2;
typedef Line2<Real> LinR2;
typedef Line2<Int > LinI2;

#endif /* LIN2_H_ */
