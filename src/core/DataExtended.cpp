#include "DataExtended.h"

namespace SEMBA {

DataExtended::DataExtended(): Data() {
	boundary = nullptr;
	grid3 = nullptr;
}

DataExtended::DataExtended(
	Boundary::Boundary* boundary, 
	const Geometry::Grid3& grid3
) :
	Data(),
	boundary(boundary)
{
	this->grid3 = std::make_unique<Geometry::Grid3>(grid3);
}
}
