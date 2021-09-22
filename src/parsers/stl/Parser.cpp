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

#include "Parser.h"

namespace SEMBA {
namespace Parsers {
namespace STL {

Data Parser::read() const {
    
    std::ifstream stl(this->filename);

    // Reads coordinates.
    std::string label;
    std::vector<Math::CVecR3> vertices;
    std::size_t nLines = std::count(std::istreambuf_iterator<char>(stl),
                                    std::istreambuf_iterator<char>(), '\n');
    vertices.reserve(nLines);
	
    stl.seekg(0); // Rewinds.
    while (stl.peek() != EOF) {
        stl >> label;
        if (label == "vertex") {
            Math::CVecR3 vertex;
            stl >> vertex(Math::Constants::x)
                >> vertex(Math::Constants::y)
                >> vertex(Math::Constants::z);
            vertices.push_back(vertex);
        }
    }
    Geometry::Coordinate::Group<Geometry::CoordR3> cG;
    cG.addPos(vertices);

    // Reads Elements and Layers.
	stl.clear();
	stl.seekg(0); // Rewinds.
	Geometry::Layer::Group<Geometry::Layer::Layer> lG;
    Geometry::Element::Group<Geometry::ElemR> eG;
    while (stl.peek() != EOF) {
        stl >> label;
        if (label == "solid") {
            std::string layerName;
            stl >> layerName;
            Geometry::Layer::Layer* lay = new Geometry::Layer::Layer(layerName);
            lG.addId(lay);
            std::string line;
            while (stl.peek() != EOF) {
                std::getline(stl, line);
                if (trim(line) == "outer loop") {
                    std::vector<const Geometry::CoordR3*> coord;
                    coord.reserve(3);
                    while (stl.peek() != EOF && label != "endloop") {
                        stl >> label;
                        if (label == "vertex") {
                            Math::CVecR3 pos;
                            stl >> pos(Math::Constants::x)
                                >> pos(Math::Constants::y)
                                >> pos(Math::Constants::z);
                            coord.push_back(cG.getPos(pos));
                        }
                    }
                    label.clear();
                    eG.addId(new Geometry::Tri3(Geometry::ElemId(0),
                                                &coord[0], lay));
                }
            }
        }
    }

    // Stores results and returns.
    Data res;
    res.mesh = new Geometry::Mesh::Geometric(Geometry::Grid3(), cG, eG, lG);

    res.physicalModels = new PhysicalModel::Group<>();
    PhysicalModel::Predefined::PEC* pec =
            new PhysicalModel::Predefined::PEC(PhysicalModel::Id(1));
    res.physicalModels->add(pec);
    res.mesh->castTo<Geometry::Mesh::Geometric>()->elems().setModel(pec);
    res.sources = new Source::Group<>();
    res.outputRequests = new OutputRequest::Group<>();

    return res;
}

void Parser::printInfo() const {
    std::cout << " --- Parser STL info --- " << std::endl;
    std::cout << " --- End of Parser STL info --- " << std::endl;
}

} /* namespace STL */
} /* namespace Parser */
} /* namespace SEMBA */
