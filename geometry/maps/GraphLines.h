/*
 * GraphLines.h
 *
 *  Created on: 15/4/2015
 *      Author: Daniel
 */

#ifndef SRC_MESHER_FDTD_MESHLINES_GRAPHLINES_H_
#define SRC_MESHER_FDTD_MESHLINES_GRAPHLINES_H_

#include <map>
#include <utility>
using namespace std;

#include "../elements/ElementsGroup.h"

#include "VertexCoord.h"
#include "VertexLine.h"

template<class T>
class GraphLines {
public:
    GraphLines();
    GraphLines(const ElementsGroup<const Line<T>>& lines);
    virtual ~GraphLines();

    UInt numCoords() const { return coords_.size(); }
    UInt numLines () const { return lines_.size();  }

    const VertexCoord<T>* coord(UInt i) const { return coords_[i]; }
    const VertexLine <T>* line (UInt i) const { return lines_[i];  }

    void splitNode(UInt i);

    void printInfo() const;

private:
    vector<VertexCoord<T>*> coords_;
    vector<VertexLine <T>*> lines_;
    map<ElementId,VertexLine<T>*> mapLines_;

    void init_(const ElementsGroup<const Line<T>>& lines);
};

#endif /* SRC_MESHER_FDTD_MESHLINES_GRAPHLINES_H_ */
