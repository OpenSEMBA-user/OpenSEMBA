#ifndef CONDITION_H_
	#include "Condition.h"
#endif

Condition::Condition() {
	elementType = ElementBase::undefined;
}

Condition::Condition(ElementBase::Type param) {
	elementType = param;
}

Condition::~Condition() {

}

Condition::Type Condition::getConditionType() const {
   return Condition::undefined;
}

ElementBase::Type Condition::getElementType() const {
    return elementType;
}
