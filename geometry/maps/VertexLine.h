/*
 * VertexLine.h
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#ifndef SRC_MESHER_FDTD_MESHLINES_VERTEXLINE_H_
#define SRC_MESHER_FDTD_MESHLINES_VERTEXLINE_H_

#include <vector>
using namespace std;

#include "../elements/Line.h"

template<class T>
class VertexCoord;

template<class T>
class VertexLine {
public:
    VertexLine(const Line<T>* line);
    VertexLine(const Line<T>* line,
               const VertexCoord<T>* nodes[2]);
    VertexLine(const VertexLine &rhs);

    const Line<T>* line() const { return line_; }

    UInt numCoords() const { return 2; }
    const VertexCoord<T>* getCoord(UInt i) const { return coords_[i]; }
    void setCoord(UInt i, const VertexCoord<T>* node);

    UInt numLines() const { return lines_.size(); }
    const VertexLine<T>* getLine(UInt i) const { return lines_[i]; }

    void constructLines();

    void printInfo() const;

private:
    const Line<T>* line_;
    const VertexCoord<T>* coords_[2];
    vector<const VertexLine<T>*> lines_;

};

#endif
