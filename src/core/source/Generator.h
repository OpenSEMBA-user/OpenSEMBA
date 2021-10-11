

#pragma once

#include "Source.h"

namespace SEMBA {
namespace Source {

class Generator : public Source<Geometry::Nod> {
public:
    typedef enum {
        voltage = 1,
        current = 2
    } Type;
    typedef enum {
        soft,
        hard
    } Hardness;

    Generator();
    Generator(Magnitude::Magnitude* magnitude,
              Geometry::Element::Group<const Geometry::Nod> elem,
              const Type& generatorType,
              const Hardness& hardness);
    Generator(const Generator& rhs);
    virtual ~Generator();

    SEMBA_CLASS_DEFINE_CLONE(Generator);

    const std::string& getName() const;
    Type getType() const;

private:
    Type type_;
    Hardness hardness_;

    std::string getTypeStr() const;
};

} /* namespace Source */
} /* namespace SEMBA */

