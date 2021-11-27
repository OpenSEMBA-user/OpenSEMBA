#pragma once

#include "Source.h"

namespace SEMBA {
namespace Source {

class OnLine : public Source {
public:
    enum class Type {
        electric = 1,
        magnetic = 2
    };
    enum class Hardness {
        hard = 1,
        soft = 2
    };
    
    OnLine(std::unique_ptr<Magnitude::Magnitude> magnitude,
           const Geometry::Element::Group<const Geometry::Lin>& elem,
           const Type& sourceType,
           const Hardness& sourceHardness);
    virtual ~OnLine() = default;
        
    std::string getName() const { return "OnLine"; };
    Type getType() const { return type_; };
    Hardness getHardness() const { return hardness_; };

private:
    Type type_;
    Hardness hardness_;
};

} /* namespace Source */
} /* namespace SEMBA */

