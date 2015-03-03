#include "CoordinateBase.h"

CoordinateBase::CoordinateBase() {

}

CoordinateBase::CoordinateBase(const CoordinateId id)
:  ClassWithIdBase<CoordinateId>(id) {

}

CoordinateBase::CoordinateBase(const CoordinateBase& rhs)
:  ClassWithIdBase<CoordinateId>(rhs) {

}

CoordinateBase::~CoordinateBase() {

}
