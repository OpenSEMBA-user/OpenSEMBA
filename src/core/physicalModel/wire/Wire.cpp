#include "Wire.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

Wire::Wire(const Id id,
           const std::string name,
           const Math::Real radius,
           const Math::Real resistance,
           const Math::Real inductance) :   
    Identifiable<Id>(id),
    PhysicalModel(name)
{
    radius_ = radius;
    seriesResistance_ = resistance;
    seriesInductance_ = inductance;
}

Wire::Wire(const Id id,
           const std::string name,
           const Math::Real radius,
           const Math::Real resistance,
           const Math::Real inductance,
           const Math::Real capacitance,
           const Math::Real pResistance,
           const Math::Real pInductance,
           const Math::Real pCapacitance) : 
    Identifiable<Id>(id),
    PhysicalModel(name)
{
    radius_ = radius;
    isSeriesParallel_ = true;
    isDispersive_ = false;
    seriesResistance_ = resistance;
    seriesInductance_ = inductance;
    seriesCapacitance_ = capacitance;
    parallelResistance_ = pResistance;
    parallelInductance_ = pInductance;
    parallelCapacitance_ = pCapacitance;
}

Wire::Wire(const Id id,
           const std::string name,
           const Math::Real radius,
           const std::string filename) :
    Identifiable<Id>(id),
    PhysicalModel(name)
{
    radius_ = radius;
    filename_ = filename;
}

Wire::Wire(const Wire& rhs) : 
    Identifiable<Id>(rhs),
    PhysicalModel(rhs) 
{
    radius_ = rhs.radius_;
    isSeriesParallel_ = rhs.isSeriesParallel_;
    isDispersive_ = rhs.isDispersive_;
    seriesResistance_ = rhs.seriesResistance_;
    seriesInductance_ = rhs.seriesInductance_;
    seriesCapacitance_ = rhs.seriesCapacitance_;
    parallelResistance_ = rhs.parallelResistance_;
    parallelInductance_ = rhs.parallelInductance_;
    parallelCapacitance_ = rhs.parallelCapacitance_;
    filename_ = rhs.filename_;
}

bool Wire::isSeriesParallel() const 
{
    return isSeriesParallel_;
}

bool Wire::isDispersive() const 
{
    return isDispersive_;
}

Math::Real Wire::getRadius() const 
{
    return radius_;
}

Math::Real Wire::getSeriesResistance() const 
{
    return seriesResistance_;
}

Math::Real Wire::getSeriesInductance() const 
{
    return seriesInductance_;
}

Math::Real Wire::getSeriesCapacitance() const 
{
    return seriesCapacitance_;
}

Math::Real Wire::getParallelResistance() const 
{
    return parallelResistance_;
}

Math::Real Wire::getParallelInductance() const 
{
    return parallelInductance_;
}

Math::Real Wire::getParallelCapacitance() const 
{
    return parallelCapacitance_;
}

std::string Wire::getFilename() const 
{
    return filename_;
}

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
