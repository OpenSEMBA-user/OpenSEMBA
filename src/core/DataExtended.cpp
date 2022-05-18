#include "DataExtended.h"

namespace SEMBA {

DataExtended::DataExtended(
	const std::vector<std::pair<PhysicalModel::Bound, PhysicalModel::Bound>>& boundary,
	const Geometry::Grid3& grid3
) :
	boundary(boundary),
	grid3(grid3)
{}
}
