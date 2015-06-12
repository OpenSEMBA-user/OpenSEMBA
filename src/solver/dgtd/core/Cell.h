/*
 * Cell.h
 *
 *  Created on: Aug 30, 2012
 *      Author: luis
 */

#ifndef CELL_H_
#define CELL_H_

#include "physicalModel/GroupPhysicalModels.h"
#include "geometry/Mesh.h"
#include "math/Simplex.h"

class Cell {
public:
	const PMVolume* material;
	//
	Cell();
	virtual ~Cell();
};

#endif /* CELL_H_ */
