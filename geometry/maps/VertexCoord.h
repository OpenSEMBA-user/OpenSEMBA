/*
 * VertexCoord.h
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#ifndef SRC_MESHER_FDTD_MESHLINES_VERTEXCOORD_H_
#define SRC_MESHER_FDTD_MESHLINES_VERTEXCOORD_H_

#include <vector>
using namespace std;

#include "../coordinates/Coordinate.h"

template<class T>
class VertexLine;

template<class T>
class VertexCoord {
public:
    VertexCoord(const Coordinate<T,3>* coord);
    VertexCoord(const Coordinate<T,3>* coord,
                const vector<const VertexLine<T>*>& line);
    VertexCoord(const VertexCoord& rhs);
    ~VertexCoord();

    const Coordinate<T,3>* coord() const { return coord_; }

    UInt numLines() const { return lines_.size(); }

    vector<const VertexLine<T>*> getLines()      const { return lines_;    }
    const VertexLine<T>*         getLine(UInt i) const { return lines_[i]; }

    void setLines(const VertexLine<T>* line);
    void setLines(const vector<const VertexLine<T>*>& line);
    void setLine(UInt i, const VertexLine<T>* line);

    void addLine(const VertexLine<T>* line);

private:
    const Coordinate<T,3>* coord_;
    vector<const VertexLine<T>*> lines_;
};

#endif
