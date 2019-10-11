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

#include "Exporter.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Hexahedron8.h"

namespace SEMBA {
namespace Exporter {

Exporter::Exporter()  {
}

Exporter::Exporter(const std::string& name)
:   Project(name) {

}

Exporter::~Exporter() {

}

//void
//Output::writeResumeFile(
//        const Real time, const FieldR3& electric, const FieldR3& magnetic) {
//    string auxStr = getFolder() + getProjectName() + ".resume";
//    char *auxChar;
//    auxChar = new char[auxStr.length() + 1];
//    strcpy(auxChar, auxStr.c_str());
//    GiD_OpenPostResultFile(auxChar, GiD_PostAscii);
//    {
//        const char * compv[] = {"Ex", "Ey", "Ez", "|E|"};
//        const char * fName = "Electric Field";
//        const char * tName = "Time";
//        GiD_BeginResult(fName, tName, time,
//                GiD_Vector, GiD_OnNodes, nullptr, nullptr, 4, compv);
//        writeAllFields(electric);
//        GiD_EndResult();
//    }
//    {
//        const char * compv[] = {"Hx", "Hy", "Hz", "|H|"};
//        const char * fName = "Magnetic Field";
//        const char * tName = "Time";
//        GiD_BeginResult(fName, tName, time,
//                GiD_Vector, GiD_OnNodes, nullptr, nullptr, 4, compv);
//        writeAllFields(magnetic);
//        GiD_EndResult();
//    }
//    GiD_ClosePostResultFile();
//    cout << " - File for resuming was succesfully written." << endl;
//}

//void Exporter::process(
//        const Math::Real time,
//        const FieldR3& electric,
//        const FieldR3& magnetic) {
//    for (std::size_t i = 0; i < result_.size(); i++) {
//        result_[i]->write(time, electric, magnetic);
//    }
//}

void Exporter::deleteExistentOutputFiles() const {
    std::string file;
    file = getFilename() + ".post.msh";
    std::remove(file.c_str());
    file = getFilename() + ".post.res";
    std::remove(file.c_str());
}

void Exporter::printInfo() const {
    std::cout << " --- Exporter info --- " << std::endl;
    std::cout << " --- End of Exporter info --- " << std::endl;
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
    const Geometry::Element::Model* boundType = mesh->bounds()(i, j);
    std::string boundName;
    if (boundType == nullptr) {
        boundName = "Undefined";
    } else {
        boundName =
            boundType->castTo<PhysicalModel::PhysicalModel>()->getName();
    }
    return boundName + "@Boundary";
}

Group::Group<Geometry::ElemR> Exporter::getGridElems(
        Geometry::CoordR3Group& cG,
        const Geometry::Grid3* grid) const {
    if (grid == nullptr) {
        return Group::Group<Geometry::ElemR>();
    }
    Group::Group<Geometry::ElemR> elem;
    Geometry::BoxR3 box = grid->getFullDomainBoundingBox();
    if (grid != nullptr) {
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
                    elem.add(new Geometry::LinR2(cG,
                                                 Geometry::ElemId(0),
                                                 Geometry::BoxR3(pMin,pMax)));
                }
            }
        }
    } else {
        elem.add(new Geometry::QuaR4(cG, Geometry::ElemId(0), box));
    }
    //elem.add(new Geometry::HexR8(cG, Geometry::ElemId(0), box));
    return elem;
}

} /* namespace Exporter */
} /* namespace SEMBA */
