#include "BoundaryCondition.h"
//
//BoundaryCondition::BoundaryCondition(
//      pair<const Volume*, UInt>& boundary,
//      const Condition* cond,
//      const bool back) {
//   cell_ = boundary.first;
//   face_ = boundary.second;
//   back_ = back;
//   condition_ = cond;
//}
//
//BoundaryCondition::~BoundaryCondition() {
//
//}
//
//void
//BoundaryCondition::printInfo() const {
//	cout << "--- BC info ---" << endl;
//	condition_->printInfo();
//	cout << "Cell Id:" << cell_ << " Face: " << face_ << endl;
//	cout << "Is back: " << back_ << endl;
//}
//
//const Volume* BoundaryCondition::getCell() const {
//   return cell_;
//}
//
//UInt BoundaryCondition::getFace() const {
//   return face_;
//}
//
//pair<const Volume*, UInt> BoundaryCondition::get() const {
//   return pair<const Volume*, UInt> (cell_, face_);
//}
//
//const Condition* BoundaryCondition::getCondition() const {
//   return condition_;
//}
//
//bool BoundaryCondition::isBack() const {
//   return back_;
//}
