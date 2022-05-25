

#include "Generator.h"

namespace SEMBA {
namespace Source {

Generator::Generator() {
    type_ = current;
    hardness_ = soft;
}

Generator::Generator(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
                     const Target& elem,
                     const Type& generatorType,
                     const Hardness& hardness)
:   SEMBA::Source::Source(magnitude, elem)
{
    //Geometry::Element::Group<const Geometry::Nod>(elems) {

    type_ = generatorType;
    hardness_ = hardness;
}

Generator::Generator(const Generator& rhs)
:   SEMBA::Source::Source(rhs)
{
    //Geometry::Element::Group<const Geometry::Nod>(rhs) {

    type_ = rhs.type_;
    hardness_ = rhs.hardness_;
}

Generator::~Generator() {

}

std::string Generator::getName() const {
    return "Generator";
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
