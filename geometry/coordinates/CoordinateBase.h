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
   
   virtual void printInfo() const = 0;
};

#endif /* COORDINATEBASE_H_ */
