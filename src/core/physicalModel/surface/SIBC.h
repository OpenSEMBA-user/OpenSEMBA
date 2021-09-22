

#pragma once

#include "math/matrix/Static.h"
#include "filesystem/Project.h"

#include "Surface.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

class SIBC : public virtual Surface {
public:
    typedef std::pair<std::complex<Math::Real>, Math::MatC22> PoleResidue;

    SIBC(const Id id,
            const std::string& name,
            const Math::MatC22& Zinfinite,
            const Math::MatC22& Zstatic,
            const std::vector<PoleResidue>& poleImpedance);
    virtual ~SIBC();

    SEMBA_CLASS_DEFINE_CLONE(SIBC);

    virtual std::size_t getNumberOfPoles() const;

    virtual void printInfo() const;

    std::vector<PoleResidue> getPoleZ() const {return poleZ_;}
    Math::MatC22 getZInfinity() const {return ZInfinity_;}
    Math::MatC22 getZLinear() const {return ZLinear_;}

private:
    Math::MatC22 ZInfinity_, ZLinear_;
    std::vector<PoleResidue> poleZ_;
};

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

