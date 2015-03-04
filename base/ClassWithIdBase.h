#ifndef CASSWITHIDBASE_H_
#define CASSWITHIDBASE_H_

#include "ClassBase.h"

template<class Id>
class ClassWithIdBase : public ClassBase {
public:
    ClassWithIdBase();
    ClassWithIdBase(const Id id);
    ClassWithIdBase(const ClassWithIdBase<Id>& rhs);
    virtual ~ClassWithIdBase();

    bool operator==(const ClassWithIdBase<Id>& rhs) const;

    Id getId() const;

    virtual ClassBase* clone()            const = 0;
    virtual ClassBase* clone(const Id id) const = 0;

protected:
    void setId(const Id id);

private:
    Id id_;
};

#include "ClassWithIdBase.hpp"
#endif /* CASSWITHIDBASE_H_ */
