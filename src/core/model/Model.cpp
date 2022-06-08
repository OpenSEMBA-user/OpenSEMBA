#include "Model.h"

namespace SEMBA {
namespace Model {

Model::Model(
	const Mesh::Unstructured& unstructuredMesh,
	const PMGroup& physicalModels
) : unstructuredMesh(unstructuredMesh),
	physicalModels(physicalModels)
{}

} // End of Model
}
