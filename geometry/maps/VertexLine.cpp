/*
 * VertexLine.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#include "VertexCoord.h"
#include "VertexLine.h"

template<class T>
VertexLine<T>::VertexLine(const Line<T>* line) {
    line_ = line;
    coords_[0] = NULL;
    coords_[1] = NULL;
}

template<class T>
VertexLine<T>::VertexLine(const Line<T>* line,
                          const VertexCoord<T>* nodes[2]) {
    line_ = line;
    coords_[0] = nodes[0];
    coords_[1] = nodes[1];
}

template<class T>
VertexLine<T>::VertexLine(const VertexLine &rhs) {
    line_ = rhs.line_;
    coords_[0] = rhs.coords_[0];
    coords_[1] = rhs.coords_[1];
}

template<class T>
void VertexLine<T>::setCoord(UInt i, const VertexCoord<T>* node) {
    coords_[i] = node;
}

template<class T>
void VertexLine<T>::constructLines() {
    lines_.clear();
    for (UInt i = 0; i < 2; i++) {
        for (UInt j = 0; j < coords_[i]->numLines(); j++) {
            if (coords_[i]->getLine(j)->line()->getId() != line_->getId()) {
                lines_.push_back(coords_[i]->getLine(j));
            }
        }
    }
}

template class VertexLine<Real>;
template class VertexLine<Int >;
