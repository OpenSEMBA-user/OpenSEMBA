/*
 * VertexCoord.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#include "VertexCoord.h"

template<class T>
VertexCoord<T>::VertexCoord(const Coordinate<T,3>* coord) {
    coord_ = coord;
}

template<class T>
VertexCoord<T>::VertexCoord(const Coordinate<T,3>* coord,
                            const vector<const VertexLine<T>*>& line) {
    coord_ = coord;
    lines_ = line;
}

template<class T>
VertexCoord<T>::VertexCoord(const VertexCoord& rhs) {
    coord_ = rhs.coord_;
    lines_ = rhs.lines_;
}

template<class T>
VertexCoord<T>::~VertexCoord() {

}

template<class T>
void VertexCoord<T>::setLines(const VertexLine<T>* line) {
    vector<const VertexLine<T>*> aux;
    aux.push_back(line);
    setLines(aux);
}

template<class T>
void VertexCoord<T>::setLines(const vector<const VertexLine<T>*>& line) {
    lines_ = line;
}

template<class T>
void VertexCoord<T>::setLine(UInt i, const VertexLine<T>* line)        {
    lines_[i] = line;
}

template<class T>
void VertexCoord<T>::addLine(const VertexLine<T>* line) {
    lines_.push_back(line);
}

template class VertexCoord<Real>;
template class VertexCoord<Int >;
