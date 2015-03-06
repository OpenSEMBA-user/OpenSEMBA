#include "CoordinateBase.h"

CoordinateBase::CoordinateBase() {

}

CoordinateBase::CoordinateBase(const CoordinateId id)
:   ClassIdBase<CoordinateId>(id) {

}

CoordinateBase::CoordinateBase(const CoordinateBase& rhs)
:   ClassIdBase<CoordinateId>(rhs) {

}

CoordinateBase::~CoordinateBase() {

}
