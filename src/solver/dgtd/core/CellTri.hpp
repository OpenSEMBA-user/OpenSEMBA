/*
 * CellTri.cpp
 *
 *  Created on: Jul 26, 2013
 *      Author: luis
 */

#include "CellTri.h"

template<int TRI_N>
CellTri<TRI_N>::CellTri() {
	// TODO Auto-generated constructor stub

}

template<int TRI_N>
CellTri<TRI_N>::~CellTri() {
	// TODO Auto-generated destructor stub
}

template<int TRI_N>
uint
CellTri<TRI_N>::getNumberOfVertices() const {
	return vertices;
}

template<int TRI_N>
CVecD3
CellTri<TRI_N>::getSideNormal(const uint s) const {
	cout << "ERROR @ CellTri:: Not Done" << endl;
	exit(-1);
}

