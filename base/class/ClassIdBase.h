#ifndef CLASSIDBASE_H_
#define CLASSIDBASE_H_

#include "IdBase.h"

#include "ClassBase.h"

template<class Id>
class ClassIdBase : public virtual ClassBase {
public:
    ClassIdBase();
    ClassIdBase(const Id id);
    ClassIdBase(const ClassIdBase<Id>& rhs);
    virtual ~ClassIdBase();

    Id   getId() const;
    void setId(const Id id);

private:
    Id id_;
};

#include "ClassIdBase.hpp"

#endif /* CLASSIDBASE_H_ */
