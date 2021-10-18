

#pragma once

#include <fstream>
#include <utility>
#include <algorithm>

#include "exporters/Exporter.h"
#include "Data.h"

namespace SEMBA {
namespace Exporters {
namespace VTK {

class Exporter : public SEMBA::Exporters::Exporter {
public:
    Exporter(const Data& smb, const std::string& fn);
    
private:
    enum CELL_TYPES {
        VTK_VERTEX               = 1,
        VTK_POLY_VERTEX          = 2,
        VTK_LINE                 = 3,
        VTK_POLY_LINE            = 4,
        VTK_TRIANGLE             = 5,
        VTK_TRIANGLE_STRIP       = 6,
        VTK_POLYGON              = 7,
        VTK_PIXEL                = 8,
        VTK_QUAD                 = 9,
        VTK_TETRA                = 10,
        VTK_VOXEL                = 11,
        VTK_HEXAHEDRON           = 12,
        VTK_WEDGE                = 13,
        VTK_PYRAMID              = 14,
        VTK_QUADRATIC_EDGE       = 21,
        VTK_QUADRATIC_TRIANGLE   = 22,
        VTK_QUADRATIC_QUAD       = 23,
        VTK_QUADRATIC_TETRA      = 24,
        VTK_QUADRATIC_HEXAHEDRON = 25
    };
    void writeMesh_(const Data* smb);
    void writeFile_(const Group::Group<const Geometry::ElemR>& elems,
                    const std::string& name,
                    std::ofstream& outMain,
                    std::size_t& part);
    std::pair<std::vector<Math::CVecR3>, 
              std::map<Geometry::CoordId, std::size_t>> getPoints_(
              const Group::Group<const Geometry::ElemR>& elems);
    void writePoints_(std::ofstream& outFile,
                      const std::vector<Math::CVecR3>& pos);
    void writeCells_(
            std::ofstream& outFile,
            const Group::Group<const Geometry::ElemR>& elems,
            std::map<Geometry::CoordId, std::size_t>& mapCoords);

    static std::string makeValid_(const std::string&);
};

} /* namespace VTK */
} /* namespace Exporters */
} /* namespace SEMBA */

