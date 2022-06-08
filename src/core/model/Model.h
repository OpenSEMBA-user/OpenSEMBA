#pragma once

#include "geometry/mesh/Unstructured.h"
#include "physicalModel/Group.h"

namespace SEMBA {
namespace Model {
	using namespace Geometry;

	class Model {
	public:
		Model() = default;
		Model(
			const Mesh::Unstructured& unstructuredMesh,
			const PMGroup& physicalModels
		);

		Mesh::Unstructured unstructuredMesh;
		PMGroup physicalModels;
	};
} // End of Model
}
