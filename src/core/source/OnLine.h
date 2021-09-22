

#pragma once

#include "Source.h"

namespace SEMBA {
namespace Source {

class OnLine : public Source<Geometry::Lin> {
public:
    enum Type {
        electric = 1,
        magnetic = 2
    };
    enum Hardness {
        hard = 1,
        soft = 2
    };
    OnLine(const Magnitude::Magnitude* magnitude,
           const Geometry::Element::Group<const Geometry::Lin>& elem,
           const Type& sourceType,
           const Hardness& sourceHardness);
    OnLine(const OnLine& rhs);
    virtual ~OnLine();

    SEMBA_CLASS_DEFINE_CLONE(OnLine);

    const std::string& getName() const;
    Type getType() const;
    Hardness getHardness() const;

    virtual void printInfo() const;
private:
    Type type_;
    Hardness hardness_;

    std::string getTypeStr() const;
    std::string getHardnessStr() const;
};

} /* namespace Source */
} /* namespace SEMBA */

