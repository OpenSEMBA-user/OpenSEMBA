
// File: physicalmodel.cpp
// =============== Includes headers ===========================================

#include "PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

PhysicalModel::PhysicalModel() {

}

PhysicalModel::PhysicalModel(const std::string& name) {
    name_ = name;
}

PhysicalModel::PhysicalModel(const PhysicalModel& rhs) {
    name_ = rhs.name_;
}

PhysicalModel::~PhysicalModel() {

}

const std::string& PhysicalModel::getName() const {
    return name_;
}
 
void PhysicalModel::setName(const std::string& newName) {
    name_ = newName;
}

void
PhysicalModel::printInfo() const {
    std::cout << " --- Physical Model Info ---" << std::endl;
    std::cout << "Id: " << getId() << std::endl;
    std::cout << "Name: " << name_ << std::endl;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
