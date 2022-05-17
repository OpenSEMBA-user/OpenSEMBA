#pragma once

#include "DataExtended.h"

namespace SEMBA {

DataExtended::DataExtended(): Data() {
	boundary = nullptr;
	grid3 = nullptr;
}

DataExtended::DataExtended(Boundary::Boundary* boundary, Geometry::Grid3* grid3) :
	Data(),
	boundary(boundary),
	grid3(grid3)
{}
}
