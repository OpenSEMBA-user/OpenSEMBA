#ifndef COORDINATEBASE_H_
#define COORDINATEBASE_H_

#include "ClassWithIdBase.h"

#ifndef COORDINATE_ERROR
#define COORDINATE_ERROR 13
#endif

CreateId(CoordinateId);

class CoordinateBase : public ClassWithIdBase<CoordinateId> {
public:
    CoordinateBase();
    CoordinateBase(const CoordinateId id);
    CoordinateBase(const CoordinateBase& rhs);
    virtual ~CoordinateBase();
    
    virtual void printInfo() const = 0;
};

#endif /* COORDINATEBASE_H_ */
