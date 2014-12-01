//============================================================================//
//   Title: BoundingBox                                                       //
//   Author: Miguel D. Ruiz - Cabello N.                                      //
//============================================================================//
// HYSTORY & VERSION:                                                         //
//    DATE beginning: 10.12.2013                                              //
//============================================================================//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "BoundingBox.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%   class BoundingBox  %%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BoundingBox::BoundingBox(
 const pair<CVecD3, CVecD3>& bounds) {
	set(bounds);
}

BoundingBox::BoundingBox(const CVecD3& min, const CVecD3& max) {
	minMax_ = pair<CVecD3, CVecD3>(min, max);
}

void BoundingBox::set(const pair<CVecD3, CVecD3>& minMax) {
	minMax_ = minMax;
}

bool BoundingBox::operator > (const BoundingBox &rhs)const{
	for (int i = 0; i < 3; i++) {
		if (minMax_.second(i) <= rhs.minMax_.second(i)) {
			return false;
		}
		if (minMax_.first(i) >= rhs.minMax_.first(i)) {
			return false;
		}
	}
	return true;
}

bool BoundingBox::operator < (const BoundingBox &lBoxMax) const{
	return lBoxMax > *this;
}

bool BoundingBox::operator == (const BoundingBox &rhs) const{
	if (minMax_.second != rhs.minMax_.second) {return false;}
	if (minMax_.first != rhs.minMax_.first) {return false;}
	return true;
}

void BoundingBox::operator += (const BoundingBox &rhs){
	for (int i = 0; i < 3; i++) {
		if (minMax_.second(i) < rhs.minMax_.second(i)) {
			minMax_.second(i) = rhs.minMax_.second(i);
		}
		if (minMax_.first(i) > rhs.minMax_.first(i)) {
			minMax_.first(i) = rhs.minMax_.first(i);
		}
	}
}

void BoundingBox::operator = (const BoundingBox &rhs){
	minMax_ = rhs.minMax_;
}

void BoundingBox::operator << (const CVecD3 &p){
	if(minMax_.second(0)<p(0)){ minMax_.second(0)=p(0); }
	if(minMax_.second(1)<p(1)){ minMax_.second(1)=p(1); }
	if(minMax_.second(2)<p(2)){ minMax_.second(2)=p(2); }

	if(minMax_.first(0)>p(0)){ minMax_.first(0)=p(0); }
	if(minMax_.first(1)>p(1)){ minMax_.first(1)=p(1); }
	if(minMax_.first(2)>p(2)){ minMax_.first(2)=p(2); }
}

bool BoundingBox::isIntersected(const BoundingBox &rhs) const {
	for (int i = 0; i < 3; i++) {
		if (minMax_.second(i) < rhs.minMax_.first(i)) {
			return false;
		}
		if (rhs.minMax_.second(i) < minMax_.first(i)) {
			return false;
		}
	}
	return true;
}

void BoundingBox::printInfo() const {
	cout<< "Bounding box info" << endl;
	cout<< "Min: ";
	minMax_.first.printInfo();
	cout<< ", Max: ";
	minMax_.second.printInfo();
	cout<< endl;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
