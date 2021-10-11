

#include "Generator.h"

namespace SEMBA {
namespace Source {

Generator::Generator() {
    type_ = current;
    hardness_ = soft;
}

Generator::Generator(Magnitude::Magnitude* magnitude,
                     Geometry::Element::Group<const Geometry::Nod> elems,
                     const Type& generatorType,
                     const Hardness& hardness)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Nod>(elems) {

    type_ = generatorType;
    hardness_ = hardness;
}

Generator::Generator(const Generator& rhs)
:   SEMBA::Source::Base(rhs),
    Geometry::Element::Group<const Geometry::Nod>(rhs) {

    type_ = rhs.type_;
    hardness_ = rhs.hardness_;
}

Generator::~Generator() {

}

const std::string& Generator::getName() const {
    const static std::string res = "Generator";
    return res;
}

Generator::Type Generator::getType() const {
    return type_;
}

std::string Generator::getTypeStr() const {
    switch (type_) {
    case voltage:
        return "Voltage";
    default:
        return "Current";
    }
}

} /* namespace Source */
} /* namespace SEMBA */
