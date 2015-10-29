// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
#include "geometry/graphs/GraphVertices.h"

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
