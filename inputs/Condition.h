#ifndef CONDITION_H_
#define CONDITION_H_

#include <stdlib.h>
#include <assert.h>

#include "../geometry/elements/Element.h"

using namespace std;

class Condition {
public:
    typedef enum {
        physicalModel,
        emSource,
        outputRequest,
        undefined
    } Type;
    Condition();
    Condition(ElementBase::Type param);
    virtual ~Condition();
    ElementBase::Type getElementType() const;
    virtual Condition::Type getConditionType() const;
private:
    ElementBase::Type elementType;
};

#endif /* CONDITION_H_ */
