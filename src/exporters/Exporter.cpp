

#include "Exporter.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Hexahedron8.h"

namespace SEMBA {
namespace Exporters {

Exporter::Exporter(const std::string& name)
:   Project(name) {}

void Exporter::deleteExistentOutputFiles() const {
    std::string file;
    file = getFilename() + ".post.msh";
    std::remove(file.c_str());
    file = getFilename() + ".post.res";
    std::remove(file.c_str());
}

std::string Exporter::getOutputfilename() const {
    std::string folder = getFolder();
    std::string output = getOutputName();
    return folder + output;
}

Geometry::ElemR* Exporter::getBoundary(
        const Math::Constants::CartesianAxis dir,
        const Math::Constants::CartesianBound pos,
        Geometry::CoordR3Group& cG,
        const Geometry::Grid3* grid,
        const Geometry::Mesh::Mesh* mesh) const {
    Geometry::BoxR3 box;
    if (grid != nullptr) {
        box = grid->getFullDomainBoundingBox();
    } else {
        box = mesh->getBoundingBox();
    }
    Geometry::BoxR3 quadBox = box.getBoundAsBox(dir,pos);
    return new Geometry::QuaR4(cG, Geometry::ElemId(0), quadBox);
}

std::string Exporter::getBoundaryName(
        const Geometry::Mesh::Structured* mesh,
        const std::size_t i,
        const std::size_t j) {
    auto boundType = mesh->bounds()(i, j);

    std::string boundName;
    if (boundType == nullptr) {
        boundName = "Undefined";
    } else {
        boundName = boundType->castTo<PhysicalModel::PhysicalModel>()->getName();
    }
    return boundName + "@Boundary";
}

ElemRGroup Exporter::getGridElems(
        Geometry::CoordR3Group& cG,
        const Geometry::Grid3* grid
) const {
    auto elem = ElemRGroup();

    if (grid == nullptr) {
        return elem;
    }

    Geometry::BoxR3 box = grid->getFullDomainBoundingBox();

    for (std::size_t d = 0; d < 3; d++) {
        // Generates grid as lines.
        for (std::size_t i = 0; i < 2; i++) {
            std::vector<Math::Real> pos = grid->getPos((d+i+1)%3);
            for (std::size_t j = 0; j < pos.size(); j++) {
                Math::CVecR3 pMin, pMax;
                pMin(d) = grid->getPos(d,Math::Constants::L);
                pMin((d+i+1)%3) = pos[j];
                pMax = pMin;
                pMin((d-i+2)%3) = grid->getPos((d-i+2)%3).front();
                pMax((d-i+2)%3) = grid->getPos((d-i+2)%3).back();
                elem.addAndAssignId(std::make_unique<Geometry::LinR2>(cG,
                                                Geometry::ElemId(0),
                                                Geometry::BoxR3(pMin,pMax)));
            }
        }
    }

    return elem;
}

} /* namespace Exporters */
} /* namespace SEMBA */
