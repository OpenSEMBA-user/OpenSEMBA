/*
 * CellTri.cpp
 *
 *  Created on: Jul 26, 2013
 *      Author: luis
 */

#include "CellTri.h"

template<Int TRI_N>
CellTri<TRI_N>::CellTri() {
	// TODO Auto-generated constructor stub

}

template<Int TRI_N>
CellTri<TRI_N>::~CellTri() {
	// TODO Auto-generated destructor stub
}

template<Int TRI_N>
UInt
CellTri<TRI_N>::getNumberOfVertices() const {
	return vertices;
}

template<Int TRI_N>
CVecR3
CellTri<TRI_N>::getSideNormal(const UInt s) const {
	cout << "ERROR @ CellTri:: Not Done" << endl;
	exit(-1);
}

