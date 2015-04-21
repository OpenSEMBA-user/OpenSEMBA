/*
 * WireGroup.h
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#ifndef SRC_MESHER_FDTD_MESHLINES_WIRESGROUP_H_
#define SRC_MESHER_FDTD_MESHLINES_WIRESGROUP_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <type_traits>
using namespace std;

#include "SmbData.h"
#include "geometry/elements/Polyline.h"
#include "geometry/maps/GraphLines.h"

template<class T>
class WiresGroup {
public:
    WiresGroup(const SmbData&);
    ~WiresGroup();

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
    GraphLines<T> constructGraph_(const SmbData&);
    void fillWiresInfo_(const GraphLines<T>&,
                        const SmbData&);
    vector<vector<const Line<T>*> > getLines_(const GraphLines<T>&);
    void getWireMats_(const PMWire*& wireMat,
                      const PMMultiport*& extremeL,
                      const PMMultiport*& extremeR,
                      const vector<const Line<T>*>&,
                      const SmbData&);
    Polyline<T>* newWire_(const vector<const Line<T>*>& lines,
                          const MatId matId);
};

#endif
