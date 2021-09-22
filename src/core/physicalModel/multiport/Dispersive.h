

#pragma once

#include "Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

class Dispersive : public virtual Multiport {
public:
    Dispersive(const Id idIn,
               const std::string nameIn,
               const std::string filename);
    Dispersive(const Dispersive&);
    virtual ~Dispersive();

    SEMBA_CLASS_DEFINE_CLONE(Dispersive);

    std::string getFilename() const;

    virtual void printInfo() const;

private:
    std::string filename_;
};

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

