#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "ProblemDescription.h"
#include "geometry/mesh/Structured.h"

namespace SEMBA {
namespace Exporters {
    using ElemRGroup = Geometry::Element::Group<Geometry::ElemR>;
    using ElemRView = std::vector<const Geometry::ElemR*>;

class Exporter : public FileSystem::Project {

public:
    Exporter(const std::string& name);
    virtual ~Exporter() = default;
    
protected:
    void deleteExistentOutputFiles() const;
    std::size_t determineStepsSaved(const Math::Real savingPeriod, const Math::Real dt) const;
    
    std::string getOutputfilename() const;
	Geometry::ElemR* getBoundary(
            const Math::Constants::CartesianAxis dir,
            const Math::Constants::CartesianBound bound,
            Geometry::CoordR3Group& cG,
            const Geometry::Grid3* grid,
            const Geometry::Mesh::Mesh* mesh) const;
    ElemRGroup getGridElems(
            Geometry::CoordR3Group& cG,
            const Geometry::Grid3* grid) const;
    static std::string getBoundaryName(
            const Geometry::Mesh::Structured* mesh,
            const std::size_t i,
            const std::size_t j);
};

} /* namespace Exporters */
} /* namespace SEMBA */

