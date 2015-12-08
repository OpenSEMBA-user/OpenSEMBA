// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#ifndef SEMBA_EXPORTER_VTK_EXPORTER_H_
#define SEMBA_EXPORTER_VTK_EXPORTER_H_

#include <fstream>
#include <utility>
#include <algorithm>

#include "exporter/Exporter.h"
#include "Data.h"

namespace SEMBA {
namespace Exporter {
namespace VTK {

class Exporter : public SEMBA::Exporter::Exporter {
public:
    Exporter(const Data* smb,
                const std::string& fn);
    virtual ~Exporter();

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
} /* namespace Exporter */
} /* namespace SEMBA */

#endif /* SEMBA_EXPORTER_VTK_EXPORTER_H_ */
