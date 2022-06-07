#include "DataExtended.h"

namespace SEMBA {

DataExtended::DataExtended(
	const std::vector<std::pair<PhysicalModel::Bound, PhysicalModel::Bound>>& boundary,
	const Geometry::Grid3& grids,
	const Source::Group<>& sources
) :
	boundary(boundary),
	grids(grids),
	sources(sources)
{}
}
