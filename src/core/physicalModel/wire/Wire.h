

#pragma once

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

class Wire : public virtual PhysicalModel {
public:
    Wire(const Id id,
         const std::string name,
         const Math::Real radius,
         const Math::Real resistance,
         const Math::Real inductance);
    Wire(const Id id,
         const std::string name,
         const Math::Real radius,
         const Math::Real resistance,
         const Math::Real inductance,
         const Math::Real capacitance,
         const Math::Real pResistance,
         const Math::Real pInductance,
         const Math::Real pCapacitance);
    Wire(const Id id,
         const std::string name,
         const Math::Real radius,
         const std::string filename);
    Wire(const Wire&);
    virtual ~Wire();

    SEMBA_CLASS_DEFINE_CLONE(Wire);

    Math::Real getRadius() const;

    bool isSeriesParallel() const;
    bool isDispersive() const;

    Math::Real getSeriesResistance() const;
    Math::Real getSeriesInductance() const;
    Math::Real getSeriesCapacitance() const;
    Math::Real getParallelResistance() const;
    Math::Real getParallelInductance() const;
    Math::Real getParallelCapacitance() const;

    std::string getFilename() const;

private:
    Math::Real radius_;
    bool isSeriesParallel_;
    bool isDispersive_;
    Math::Real seriesResistance_;    // Resistance per meter.
    Math::Real seriesInductance_;    // Inductance per meter.
    Math::Real seriesCapacitance_;   // Capacitance per meter.
    Math::Real parallelResistance_;  // Resistance per meter.
    Math::Real parallelInductance_;  // Inductance per meter.
    Math::Real parallelCapacitance_; // Capacitance per meter.
    std::string filename_;
};

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

