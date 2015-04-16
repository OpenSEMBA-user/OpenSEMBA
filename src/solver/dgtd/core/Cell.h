/*
 * Cell.h
 *
 *  Created on: Aug 30, 2012
 *      Author: luis
 */

#ifndef CELL_H_
#define CELL_H_

#include "../../../common/inputs/physicalModel/PhysicalModelGroup.h"
#include "../../../common/geometry/Mesh.h"
#include "../../../common/math/Simplex.h"

#ifdef USE_OPENMP
	#include <omp.h>
#endif

#ifndef CELL_ERROR
	#define CELL_ERROR 45
#endif
#ifdef USE_OPENMP
	#define CELL_USE_OPENMP
#endif

class Cell {
public:
	const PMVolume* material;
	//
	Cell();
	virtual ~Cell();
};

#endif /* CELL_H_ */
