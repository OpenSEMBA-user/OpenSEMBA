#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

class PriorityMaterial : public virtual PhysicalModel {
public:
	PriorityMaterial(const Id& matId, const std::string& name, int priority);
	PriorityMaterial(const PriorityMaterial&);

	virtual ~PriorityMaterial() = default;

	virtual std::unique_ptr<PhysicalModel> clone() const override {
		return std::make_unique<PriorityMaterial>(*this);
	}


private:
	int priority_ = 0;
};


} /* namespace PhysicalModel */
} /* namespace SEMBA */

