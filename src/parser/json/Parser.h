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

#ifndef SEMBA_PARSER_JSON_PARSER_H_
#define SEMBA_PARSER_JSON_PARSER_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "geometry/mesh/Geometric.h"
#include "physicalModel/bound/PEC.h"
#include "physicalModel/bound/PMC.h"
#include "physicalModel/bound/PML.h"
#include "physicalModel/bound/Periodic.h"
#include "physicalModel/bound/Mur1.h"
#include "physicalModel/bound/Mur2.h"
#include "physicalModel/multiport/Multiport.h"
#include "physicalModel/surface/Multilayer.h"
#include "physicalModel/surface/SIBC.h"
#include "physicalModel/volume/Dispersive.h"
#include "physicalModel/volume/Anisotropic.h"
#include "source/Dipole.h"
#include "source/Generator.h"
#include "source/OnLine.h"
#include "source/PlaneWave.h"
#include "source/port/Waveguide.h"
#include "source/port/TEM.h"
#include "Data.h"
#include "util/ProgressBar.h"

#include "parser/Parser.h"
#include "json.hpp"


namespace SEMBA {
namespace Parser {
namespace JSON {

using json = nlohmann::json;

class Parser : public SEMBA::Parser::Parser {
public:
    Data read(std::istream& inputFileStream) const;

private:
    typedef enum {
        outRqOnPoint,
        outRqOnLine,
        outRqOnSurface,
        outRqOnLayer,
        bulkCurrentOnSurface,
        bulkCurrentOnLayer,
        farField
    } OutputType;
    typedef enum {
        sibc,
        multilayer
    } SIBCType;
    typedef enum {
        rectangular
    } WaveportShape;

    struct ParsedElementIds {
        Geometry::ElemId elemId;
        MatId mat;
        Geometry::Layer::Id layer;
        std::vector<Geometry::CoordId> v;
    };

    struct ParsedElementPtrs {
        const Geometry::Layer::Layer* layerPtr;
        const PhysicalModel::PhysicalModel* matPtr;
        std::vector<const Geometry::CoordR3*> vPtr;
    };

    static Solver::Info* readSolver(const json&);
    static Solver::Settings readSolverSettings(const json&);
    static PhysicalModel::Group<>* readPhysicalModels(const json&);
    static Geometry::Mesh::Geometric* readGeometricMesh(
            const PhysicalModel::Group<>&, const json&);
    static Source::Group<>* readSources(
            Geometry::Mesh::Geometric& mesh, const json&);
    static OutputRequest::Group<>* readOutputRequests(
            Geometry::Mesh::Geometric& mesh, const json&);

    static PhysicalModel::Surface::Multilayer* readMultilayerSurface(
            const MatId id,
            const std::string& name,
            const json& layers);

    static Geometry::Grid3 readGrids(const json&);
    static Geometry::Layer::Group<> readLayers(const json&);
    static Geometry::Coordinate::Group<Geometry::CoordR3> readCoordinates(
            const json&);
    static Geometry::Element::Group<Geometry::ElemR> readElements(
            const PhysicalModel::Group<>& physicalModels,
            const Geometry::Layer::Group<>&,
            const Geometry::CoordR3Group&,
            const json&);
    static ParsedElementIds readElementIds(
            const std::string& str, size_t numberOfVertices);
    static ParsedElementPtrs convertElementIdsToPtrs(
            const ParsedElementIds& elemIds,
            const PhysicalModel::Group<>& physicalModels,
            const Geometry::Layer::Group<>& layers,
            const Geometry::Coordinate::Group<Geometry::CoordR3>& coords);

    static Source::PlaneWave* readPlanewave(
            Geometry::Mesh::Geometric& mesh, const json&);
    static Source::Port::Waveguide* readPortWaveguide(
            Geometry::Mesh::Geometric& mesh, const json&);
    static Source::Port::TEM* readPortTEM(
            Geometry::Mesh::Geometric& mesh, const json&);
    static Source::Generator* readGenerator(
            Geometry::Mesh::Geometric& mesh, const json&);
    static Source::OnLine* readSourceOnLine(
            Geometry::Mesh::Geometric& mesh, const json&);
    static Source::Magnitude::Magnitude* readMagnitude(const json&);

    static PhysicalModel::PhysicalModel* readPhysicalModel(
            const json& material);

    static OutputRequest::Base* readOutputRequest(
            Geometry::Mesh::Geometric& mesh, const json&);

    static OutputRequest::Domain readDomain(const json&);
    static Math::Axis::Local strToLocalAxes(const std::string& str);

    static bool checkVersionCompatibility(const std::string& version);

    static Geometry::Element::Group<> boxToElemGroup(
            Geometry::Mesh::Geometric& mesh,
            const std::string& line);

    static Geometry::Element::Group<const Geometry::Nod> readAsNodes(
            Geometry::Mesh::Geometric& mesh, const json&);

    static OutputRequest::Base::Type strToOutputType(std::string label);
    static SIBCType                  strToSIBCType(std::string str);
    static OutputType                strToGiDOutputType(std::string label);

    static Source::Generator::Type     strToGeneratorType(std::string label);
    static Source::Generator::Hardness strToGeneratorHardness(std::string str);
    static Source::OnLine::Type        strToNodalType(std::string label);

    static Source::OnLine::Hardness    strToNodalHardness(std::string label);
    static Source::Port::TEM::ExcitationMode
                                       strToTEMMode(std::string);
    static Source::Port::Waveguide::ExcitationMode
                                       strToWaveguideMode(std::string);

    static PhysicalModel::PhysicalModel::Type        strToMaterialType(
            std::string label);
    static PhysicalModel::Multiport::Multiport::Type strToMultiportType(
            std::string label);
    static PhysicalModel::Volume::Anisotropic::Model strToAnisotropicModel(
            std::string label);

    static Math::CVecI3 strToCVecI3(const std::string& str);
    static Math::CVecR3 strToCVecR3(const std::string& str);
    static Math::Constants::CartesianAxis strToCartesianAxis(std::string);

    static std::pair<Math::CVecR3, Math::CVecR3> strToBox(
            const std::string& str);
    static const PhysicalModel::Bound::Bound*    strToBoundType(std::string str);
};

template<typename T>
Geometry::Element::Group<T> readElemIdsAsGroupOf(
		Geometry::Mesh::Geometric& mesh, const json& j) {
	Geometry::Element::Group<T> geometricElements;
	for (auto it = j.begin(); it != j.end(); ++it) {
		geometricElements.add(mesh.elems().getId(Geometry::ElemId(it->get<int>()) ));
	}
	return geometricElements;
}

} /* namespace GiD */
} /* namespace Parser */
} /* namespace SEMBA */

#endif /* SEMBA_PARSER_GID_PARSER_H_ */
