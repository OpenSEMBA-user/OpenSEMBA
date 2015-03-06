#ifndef CASSIDBASE_H_
#define CASSIDBASE_H_

#include "IdBase.h"
#include "GroupId.h"

#include "ClassBase.h"

template<class Id>
class ClassIdBase : public ClassBase {
    template<typename T, class GId>
    friend class GroupId;
public:
    ClassIdBase();
    ClassIdBase(const Id id);
    ClassIdBase(const ClassIdBase<Id>& rhs);
    virtual ~ClassIdBase();

    bool operator==(const ClassIdBase<Id>& rhs) const;

    Id getId() const;

    virtual ClassBase* clone()            const = 0;
    virtual ClassBase* clone(const Id id) const = 0;

protected:
    void setId(const Id id);

private:
    Id id_;
};

#include "ClassIdBase.hpp"
#endif /* CASSIDBASE_H_ */
