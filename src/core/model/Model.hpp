#include "Model.h"

namespace SEMBA {
namespace Model {

template<typename M>
Model<M>::Model(
	const M& mesh,
	const PMGroup& physicalModels
) : mesh(mesh),
	physicalModels(physicalModels)
{
	this->mesh.reassignPointers(this->physicalModels);
}

template<typename M>
Model<M>& Model<M>::operator=(const Model& rhs) {
	mesh = rhs.mesh;
	physicalModels = rhs.physicalModels;

	mesh.reassignPointers(physicalModels);

	return *this;
}

}
}
