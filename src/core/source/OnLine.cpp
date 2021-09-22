

#include "OnLine.h"

namespace SEMBA {
namespace Source {

OnLine::OnLine(const Magnitude::Magnitude* magnitude,
               const Geometry::Element::Group<const Geometry::Lin>& elem,
               const Type& sourceType,
               const Hardness& sourceHardness)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Lin>(elem) {

    type_ = sourceType;
    hardness_ = sourceHardness;
}

OnLine::OnLine(const OnLine& rhs)
:   SEMBA::Source::Base(rhs),
    Geometry::Element::Group<const Geometry::Lin>(rhs) {

    type_ = rhs.type_;
    hardness_ = rhs.hardness_;
}

OnLine::~OnLine() {

}

const std::string& OnLine::getName() const {
    const static std::string res = "OnLine";
    return res;
}

OnLine::Type OnLine::getType() const {
    return type_;
}

OnLine::Hardness OnLine::getHardness() const {
    return hardness_;
}

void OnLine::printInfo() const {
    std::cout<< " --- Nodal info --- " << std::endl;
    SEMBA::Source::Base::printInfo();
    std::cout<< "Type: " << getTypeStr() << std::endl;
    std::cout<< "Hardness: " << getHardnessStr() << std::endl;
}

std::string OnLine::getTypeStr() const {
    switch (type_) {
    case electric:
        return "Electric Field";
    default:
        return "Magnetic Field";
    }
}

std::string OnLine::getHardnessStr() const {
    switch (type_) {
    case hard:
        return "Hard";
    default:
        return "Soft";
    }
}

} /* namespace Source */
} /* namespace SEMBA */
