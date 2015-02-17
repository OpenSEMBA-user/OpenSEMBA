// File: physicalmodel.cpp
// =============== Includes headers ===========================================

#include "PhysicalModel.h"

const double PhysicalModel::eps0 = VACUUM_PERMITTIVITY;
const double PhysicalModel::mu0 =VACUUM_PERMEABILITY;


PhysicalModel::PhysicalModel() {
	id_ = 0;
}

PhysicalModel::~PhysicalModel() {

}

const string&
PhysicalModel::getName() const {
	return name_;
}
 
double
PhysicalModel::getImpedance() const {
	cerr << "ERROR @ PhysicalModel::getImpedance" << endl;
	printInfo();
	exit(PHYSICALMODEL_ERROR);
}
 
double
PhysicalModel::getAdmitance() const {
	cerr << "ERROR @ PhysicalModel::getAdmitance" << endl;
	printInfo();
	exit(PHYSICALMODEL_ERROR);
}
 
bool
PhysicalModel::isPML() const {
	return false;
}

void
PhysicalModel::printInfo() const {
	cerr << "ERROR@PhysicalModel::printInfo()" << endl;
	cerr << "Unable to find printInfo function." << endl;
	exit(PHYSICALMODEL_ERROR);
}

PhysicalModel::PhysicalModel(const uint id, const string& name) {
   id_ = id;
   name_ = name;
}
