

#pragma once

#include "Source.h"

namespace SEMBA {
namespace Source {

class Generator : public Source {
public:
    typedef enum {
        voltage = 1,
        current = 2
    } Type;
    typedef enum {
        soft,
        hard
    } Hardness;

    Generator(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
              const Target& elem,
              const Type& generatorType,
              const Hardness& hardness);
    Generator(const Generator& rhs);
    virtual ~Generator() = default;

    virtual std::unique_ptr<Source> clone() const override {
        return std::make_unique<Generator>(*this);
    }

    std::string getName() const;
    Type getType() const;

private:
    Type type_;
    Hardness hardness_;

    std::string getTypeStr() const;
};

} /* namespace Source */
} /* namespace SEMBA */

