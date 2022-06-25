#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

class PriorityMaterial : public virtual PhysicalModel {
public:
	PriorityMaterial(
		const Id& matId, 
		const std::string& name, 
		int priority, 
		bool treatAsVolume);

	virtual ~PriorityMaterial() = default;

	bool operator==(const PriorityMaterial&) const;

	virtual std::unique_ptr<PhysicalModel> clone() const override {
		return std::make_unique<PriorityMaterial>(*this);
	}

	int getPriority() const { return priority_; }
	bool isTreatAsVolume() const { return treatAsVolume_; }

private:
	int priority_{ 0 };
	bool treatAsVolume_{ false };
};

} /* namespace PhysicalModel */
} /* namespace SEMBA */

