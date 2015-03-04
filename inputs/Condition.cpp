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

Condition::Type Condition::getConditionType() const {
   return Condition::undefined;
}

Element::Type Condition::getElementType() const {
    return elementType;
}
