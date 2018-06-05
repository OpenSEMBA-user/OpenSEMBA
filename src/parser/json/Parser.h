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
#include <algorithm>

#include "geometry/mesh/Geometric.h"
#include "physicalModel/bound/PEC.h"
#include "physicalModel/bound/PMC.h"
#include "physicalModel/bound/PML.h"
#include "physicalModel/bound/Periodic.h"
#include "physicalModel/bound/Mur1.h"
#include "physicalModel/bound/Mur2.h"
#include "physicalModel/multiport/Multiport.h"
#include "physicalModel/surface/Multilayer.h"
#include "physicalModel/surface/SIBCFile.h"
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

    static Geometry::Element::Group<Geometry::Nod> readCoordIdAsNodes(
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

    static Math::CVecI3 strToCVecI3(std::string str);
    static Math::CVecR3 strToCVecR3(std::string str);
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

template<typename T>
Geometry::Element::Group<Geometry::ElemR> readElemStrAs(
        const PhysicalModel::Group<>& mG,
        const Geometry::Layer::Group<>& lG,
        const Geometry::CoordR3Group& cG,
        const json& e) {
    Geometry::Element::Group<Geometry::ElemR> res;

    for (json::const_iterator it = e.begin(); it != e.end(); ++it) {

        Geometry::ElemId elemId;
        MatId matId;
        Geometry::Layer::Id layerId;
        std::vector<Geometry::CoordId> vId;

        std::stringstream ss(it->get<std::string>());
        ss >> elemId >> matId >> layerId;
        vId.resize(T::sizeOfCoordinates);
        for (std::size_t j = 0; j < T::sizeOfCoordinates; j++) {
            ss >> vId[j];
        }

        const Geometry::Layer::Layer* layerPtr;
        const PhysicalModel::PhysicalModel* matPtr;
        std::vector<const Geometry::CoordR3*> vPtr;

        if (matId != MatId(0)) {
            matPtr = mG.getId(matId);
        } else {
            matPtr = nullptr;
        }
        if (layerId != Geometry::LayerId(0)) {
            layerPtr = lG.getId(layerId);
        } else {
            layerPtr = nullptr;
        }
        vPtr.resize(vId.size(), nullptr);
        for (size_t i = 0; i < vId.size(); ++i) {
            vPtr[i] = cG.getId(vId[i]);
        }

        res.add(new T(elemId, vPtr.data(), layerPtr, matPtr));
    }

    return res;
}

} /* namespace JSON */
} /* namespace Parser */
} /* namespace SEMBA */

#endif /* SEMBA_PARSER_GID_PARSER_H_ */
