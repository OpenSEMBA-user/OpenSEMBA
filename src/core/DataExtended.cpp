#include "DataExtended.h"

namespace SEMBA {

DataExtended::DataExtended(
	const std::vector<std::pair<PhysicalModel::Bound, PhysicalModel::Bound>>& boundary,
	const Geometry::Grid3& grid3
) :
	Data(),
	boundary(boundary)
{
	this->grid3 = std::make_unique<Geometry::Grid3>(grid3);
}
}
