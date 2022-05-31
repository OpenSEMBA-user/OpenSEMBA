

#include "Generator.h"

namespace SEMBA {
namespace Source {

Generator::Generator(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
                     const Target& elem,
                     const Type& generatorType,
                     const Hardness& hardness)
:   SEMBA::Source::Source(magnitude, elem)
{
    type_ = generatorType;
    hardness_ = hardness;
}

Generator::Generator(const Generator& rhs)
:   SEMBA::Source::Source(rhs)
{
    type_ = rhs.type_;
    hardness_ = rhs.hardness_;
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
