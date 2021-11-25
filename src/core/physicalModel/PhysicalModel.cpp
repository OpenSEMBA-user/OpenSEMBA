#include "PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

PhysicalModel::PhysicalModel(const std::string& name) 
{
    name_ = name;
}

PhysicalModel::PhysicalModel(const Id& id, const std::string& name) :
    name_{name},
    Identifiable<Id>{id}
{

}

const std::string& PhysicalModel::getName() const {
    return name_;
}
 
void PhysicalModel::setName(const std::string& newName) {
    name_ = newName;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
