#pragma once

#include "Port.h"

namespace SEMBA {
namespace Source {
namespace Port {

class TEM : public Port {
public:
    typedef enum {
        voltage,
        current
    } ExcitationMode;

    TEM(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
        const Target& elem,
        const ExcitationMode excitationMode);
    TEM(const TEM& rhs);
    virtual ~TEM() = default;

    //virtual std::unique_ptr<Source> clone() const override {
    //    return std::make_unique<TEM>(*this);
    //}

    std::string getName() const { return "TEM_Port"; };

    ExcitationMode getExcitationMode() const;
    static std::string toStr(const ExcitationMode& excitationMode);
private:
    ExcitationMode excitationMode_;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

