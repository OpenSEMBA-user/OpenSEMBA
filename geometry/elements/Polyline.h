/*
 * Polyline.h
 *
 *  Created on: Apr 15, 2015
 *      Author: damarro
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include <vector>
using namespace std;

#include "Line.h"

class PolylineBase : public virtual LineBase {
public:
    PolylineBase() {};
    virtual ~PolylineBase() {};
};

template<class T>
class Polyline : public virtual Line<T>,
                 public virtual PolylineBase {
public:
    Polyline();
    Polyline(const ElementId id,
             const vector<const Coordinate<T,3>*>& v,
             const LayerId layerId = LayerId(0),
             const MatId   matId   = MatId(0));
    Polyline(const Polyline<T>& rhs);
    virtual ~Polyline();
    
    DEFINE_CLONE(Polyline<T>);

    inline UInt numberOfCoordinates() const { return v_.size(); }

    const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideV(const UInt f,
                                    const UInt i) const;

    const Coordinate<T,3>* getVertex    (const UInt i) const;
    const Coordinate<T,3>* getSideVertex(const UInt f,
                                         const UInt i) const;

    vector<const Coordinate<T,3>*> getVertices() const;
    vector<const Coordinate<T,3>*> getSideVertices(const UInt face) const;

    void setV(const UInt i, const Coordinate<T,3>* coord);

    void printInfo() const;

private:
    vector<const Coordinate<T,3>*> v_;
};

typedef PolylineBase   Polylin;
typedef Polyline<Real> PolylinR;
typedef Polyline<Int > PolylinI;

#endif /* POLYLINE_H_ */
