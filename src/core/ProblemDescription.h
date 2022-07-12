#pragma once

#include "geometry/Grid.h"
#include "physicalModel/Bound.h"
#include "source/Group.h"
#include "outputRequest/Group.h"
#include "model/Model.h"

#include "nlohmann/json.hpp"

namespace SEMBA {
	class ProblemDescription {
	public:
		Geometry::Grid3 grids;
		SourceGroup sources;
		nlohmann::json analysis;

		Model::UnstructuredModel model;

		OutputRequestGroup outputRequests;

		ProblemDescription() = default;
		ProblemDescription(const ProblemDescription& rhs);

		ProblemDescription& operator=(const ProblemDescription& rhs);
	};
}
