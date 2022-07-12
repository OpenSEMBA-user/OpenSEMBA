#pragma once

#include "geometry/mesh/Unstructured.h"
#include "physicalModel/Group.h"

namespace SEMBA {
namespace Model {

using namespace Geometry;

template<typename M = Mesh::Unstructured>
class Model {
public:
	Model() = default;
	Model(const M&, const PMGroup&);
	Model& operator=(const Model& rhs);

	M mesh;
	PMGroup physicalModels;
};

typedef Model<Mesh::Unstructured> UnstructuredModel;
typedef Model<Mesh::Structured> StructuredModel;

}
}

#include "Model.hpp"