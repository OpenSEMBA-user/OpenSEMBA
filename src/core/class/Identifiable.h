

#pragma once

#include "Class.h"
#include "Shareable.h"

namespace SEMBA {
namespace Class {

template<class Id>
class Identifiable : public virtual Class,
                     public virtual Shareable {
public:
    Identifiable();
    Identifiable(const Id id);
    Identifiable(const Identifiable<Id>& rhs);
    virtual ~Identifiable();

    Id   getId() const;
    void setId(const Id id);

    virtual bool operator==(const Identifiable& rhs) const;

private:
    Id id_;
};

} /* namespace Class */
} /* namespace SEMBA */

#include "Identifiable.hpp"

