#pragma once

#include "Class.h"
#include "Shareable.h"

namespace SEMBA {
namespace Class {

template<class Id>
class Identifiable : public virtual Class,
                     public virtual Shareable {
public:
    Identifiable() = default;
    Identifiable(const Id& id);
    Identifiable(const Identifiable& rhs) {
        id_ = rhs.id_;
    }
    virtual ~Identifiable() = default;

    Id   getId() const;
    void setId(const Id& id);

    virtual bool operator==(const Identifiable& rhs) const;

private:
    Id id_{ 0 };
};

} /* namespace Class */
} /* namespace SEMBA */

#include "Identifiable.hpp"

