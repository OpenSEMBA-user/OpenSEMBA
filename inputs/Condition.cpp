#ifndef CONDITION_H_
	#include "Condition.h"
#endif

Condition::Condition() {
	elementType = Element::undefined;
}

Condition::Condition(Element::Type param) {
	elementType = param;
}

Condition::~Condition() {

}

bool
Condition::isSMA() const {
	return false;
}

bool
Condition::isPEC() const {
	return false;
}

bool
Condition::isPMC() const {
	return false;
}

unsigned int
Condition::getId() const {
	return 0;
}

Condition::Type
Condition::getConditionType() const {
   return Condition::undefined;
}
