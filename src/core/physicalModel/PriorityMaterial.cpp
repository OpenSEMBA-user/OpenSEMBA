#include "PriorityMaterial.h"

namespace SEMBA {
namespace PhysicalModel {

PriorityMaterial::PriorityMaterial(
	const Id& matId, 
	const std::string& name, 
	int priority,
	bool treatAsVolume) :
	Identifiable<Id>(matId),
	PhysicalModel(name),
	priority_(priority),
	treatAsVolume_(treatAsVolume)
{}

bool PriorityMaterial::operator==(const PriorityMaterial& rhs) const
{
	bool res{ true };
	res &= PhysicalModel::operator==(rhs);
	res &= priority_ == rhs.priority_;
	res &= treatAsVolume_ == rhs.treatAsVolume_;

	return res;
}

}
}