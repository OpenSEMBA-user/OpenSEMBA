

#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {

class Bound : public virtual PhysicalModel {
public:
    enum class Type {
        mur1,
        mur2,
        pec,
        pmc,
        periodic,
        pml,
        sma
    };
    
    Bound(Id id, Type type);
    virtual ~Bound() = default;

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<Bound>(*this);
    }

    Type getType() const;
private:
    Type type;
};

} /* namespace PhysicalModel */
} /* namespace SEMBA */

