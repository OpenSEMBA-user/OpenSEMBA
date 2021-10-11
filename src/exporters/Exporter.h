#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Data.h"
#include "geometry/mesh/Structured.h"

namespace SEMBA {
namespace Exporters {

class Exporter : public FileSystem::Project {
public:
    Exporter();
    Exporter(const std::string& name);
    
protected:
    void deleteExistentOutputFiles() const;
    std::size_t determineStepsSaved(
            const Math::Real savingPeriod,
            const Math::Real dt) const;
protected:
    std::string getOutputfilename() const;
	Geometry::ElemR* getBoundary(
            const Math::Constants::CartesianAxis dir,
            const Math::Constants::CartesianBound bound,
            Geometry::CoordR3Group& cG,
            const Geometry::Grid3* grid,
            const Geometry::Mesh::Mesh* mesh) const;
    Group::Group<Geometry::ElemR> getGridElems(
            Geometry::CoordR3Group& cG,
            const Geometry::Grid3* grid) const;
    static std::string getBoundaryName(
            const Geometry::Mesh::Structured* mesh,
            const std::size_t i,
            const std::size_t j);
};

} /* namespace Exporters */
} /* namespace SEMBA */

