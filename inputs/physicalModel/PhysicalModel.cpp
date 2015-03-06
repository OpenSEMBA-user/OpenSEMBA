// File: physicalmodel.cpp
// =============== Includes headers ===========================================

#include "PhysicalModel.h"

PhysicalModel::PhysicalModel(const uint id, const string& name)
:  ClassWithIdBase<uint>(id) {
   name_ = name;
}

PhysicalModel::~PhysicalModel() {

}

const string&
PhysicalModel::getName() const {
	return name_;
}
 
void PhysicalModel::setName(const string& newName) {
    name_ = newName;
}

void
PhysicalModel::printInfo() const {
	cout << " --- Physical Model Info ---" << endl;
	cout << "Id: " << getId() << endl;
	cout << "Name: " << name_ << endl;
}

