#ifndef COORDINATEBASE_H_
#define COORDINATEBASE_H_

#include "ClassIdBase.h"

CreateId(CoordinateId);

class CoordinateBase : public ClassIdBase<CoordinateId> {
public:
    CoordinateBase();
    CoordinateBase(const CoordinateId id);
    CoordinateBase(const CoordinateBase& rhs);
    virtual ~CoordinateBase();
    
    virtual void printInfo() const = 0;
};

#endif /* COORDINATEBASE_H_ */
