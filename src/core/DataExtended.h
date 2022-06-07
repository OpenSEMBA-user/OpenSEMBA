#pragma once

#include "geometry/Grid.h"
#include "physicalModel/Bound.h"
#include "source/Group.h"

#include "nlohmann/json.hpp"

namespace SEMBA {
	class DataExtended {
	public:
		// 1:1
		std::vector<std::pair<PhysicalModel::Bound, PhysicalModel::Bound>> boundary;

		Geometry::Grid3 grids;
		SourceGroup sources;
		// Analysis debe ser directo como el `struct` que existe ahora mismo
		nlohmann::json analysis;


		// ----------- Model
			// Mesh siempre como no estructurada
			// PhysicalModels

		// ----------- Probes
			// si "coordinates" -> Creas coordenada -> Creas Nodo -> Se lo metes a la malla -> Se reasigna como target del outputRequest inicial

		

		DataExtended() = default;
		DataExtended(
			const std::vector<std::pair<PhysicalModel::Bound, PhysicalModel::Bound>>& boundary,
			const Geometry::Grid3& grids,
			const Source::Group<>& sources,
			const nlohmann::json& analysis
		);
	};
}
