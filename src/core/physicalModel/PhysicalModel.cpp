
// File: physicalmodel.cpp
// =============== Includes headers ===========================================

#include "PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

PhysicalModel::PhysicalModel(const std::string& name) 
{
    name_ = name;
}

const std::string& PhysicalModel::getName() const {
    return name_;
}
 
void PhysicalModel::setName(const std::string& newName) {
    name_ = newName;
}


} /* namespace PhysicalModel */
} /* namespace SEMBA */
