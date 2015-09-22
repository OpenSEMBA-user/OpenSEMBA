/*
 * GroupWires.h
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#ifndef COMMON_GROUPWIRES_H_
#define COMMON_GROUPWIRES_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <type_traits>
using namespace std;

#include "SmbData.h"
#include "geometry/elements/Polyline.h"
#include "geometry/maps/GraphVertices.h"

template<class T>
class GroupWires {
public:
    typedef GraphVertices<Line<T>,Coordinate<T,3>> Graph;

    GroupWires(const SmbData&);
    ~GroupWires();

    UInt numberOfWires() const { return wires_.size(); }
    const Polyline<T>* getWire(UInt i) const { return wires_[i]; }

    PMWireExtremes*       getMat(MatId id)       { return mats_.at(id); }
    const PMWireExtremes* getMat(MatId id) const { return mats_.at(id); }

    const vector<ElementId>& getIdsOfWire(UInt i) const { return wiresIds_[i]; }

    void printInfo() const;

private:
    vector<Polyline<T>*> wires_;
    vector<vector<ElementId>> wiresIds_;

    map<MatId, PMWireExtremes*> mats_;

    void init_(const SmbData&);
    Graph constructGraph_(const SmbData&);
    void fillWiresInfo_(const Graph&,
                        const SmbData&);
    vector<vector<const Line<T>*> > getLines_(const Graph&);
    void getWireMats_(const PMWire*& wireMat,
                      const PMMultiport*& extremeL,
                      const PMMultiport*& extremeR,
                      const vector<const Line<T>*>&,
                      const SmbData&);
    Polyline<T>* newWire_(const vector<const Line<T>*>& lines,
                          const MatId matId);
};

#endif
