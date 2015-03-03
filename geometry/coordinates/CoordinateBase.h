#ifndef COORDINATEBASE_H_
#define COORDINATEBASE_H_

#include "ClassWithIdBase.h"

typedef unsigned CoordinateId;

class CoordinateBase : public ClassWithIdBase<CoordinateId> {
public:
   CoordinateBase();
   CoordinateBase(const CoordinateId id);
   CoordinateBase(const CoordinateBase& rhs);
   virtual ~CoordinateBase();
};

#endif /* COORDINATEBASE_H_ */
