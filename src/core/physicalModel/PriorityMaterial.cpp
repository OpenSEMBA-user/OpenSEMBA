#include "PriorityMaterial.h"

namespace SEMBA {
namespace PhysicalModel {

PriorityMaterial::PriorityMaterial(const Id& matId, const std::string& name, int priority) :
	Identifiable<Id>(matId),
	PhysicalModel(name),
	priority_(priority)
{}

PriorityMaterial::PriorityMaterial(const PriorityMaterial& rhs) :
	Identifiable<Id>(rhs),
	PhysicalModel(rhs) 
{
	priority_ = rhs.priority_;
}

}
}