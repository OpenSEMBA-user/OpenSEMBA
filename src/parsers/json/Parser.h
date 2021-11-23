
#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "geometry/mesh/Geometric.h"
#include "physicalModel/Bound.h"
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

#include "parsers/Parser.h"

namespace SEMBA {
namespace Parsers {
namespace JSON {

class Parser : public SEMBA::Parsers::Parser {
    typedef nlohmann::json json;
public:
    Parser(const std::string& filename) : SEMBA::Parsers::Parser(filename) {};
    Data read() const;
    
private:
    json readSolverOptions(const json&) const;
    PMGroup readPhysicalModels(const json&) const;
    Geometry::Mesh::Geometric* readGeometricMesh(const PhysicalModel::Group<>&, const json&) const;
	void readConnectorOnPoint(PMGroup& pMG, Geometry::Mesh::Geometric& mesh,  const json&) const;
    Source::Group<>* readSources(Geometry::Mesh::Geometric& mesh, const json&) const;
    OutputRequest::Group<>* readOutputRequests(Geometry::Mesh::Geometric& mesh, const json&) const;

    std::unique_ptr<PhysicalModel::Surface::Multilayer> 
        readMultilayerSurface(const json& layers) const;

    Geometry::Grid3 readGrids(const json&) const;
    Geometry::Grid3 buildGridFromFile(const FileSystem::Project& file) const;

    Geometry::Layer::Group<> readLayers(const json&) const;
    
    Geometry::Coordinate::Group<Geometry::CoordR3> readCoordinates(const json&) const;
    Geometry::Element::Group<Geometry::ElemR> readElements(
            const PhysicalModel::Group<>& physicalModels,
            const Geometry::Layer::Group<>&,
            const Geometry::CoordR3Group&,
            const json&) const;

    static Source::PlaneWave* readPlanewave(Geometry::Mesh::Geometric& mesh, const json&);
    static Source::Port::Waveguide* readPortWaveguide(Geometry::Mesh::Geometric& mesh, const json&);
    static Source::Port::TEM* readPortTEM(Geometry::Mesh::Geometric& mesh, const json&);
    static Source::Generator* readGenerator(Geometry::Mesh::Geometric& mesh, const json&);
    static Source::OnLine* readSourceOnLine(Geometry::Mesh::Geometric& mesh, const json&);
    static Source::Magnitude::Magnitude* readMagnitude(const json&);

    std::unique_ptr<PhysicalModel::PhysicalModel> readPhysicalModel(const json& material) const;

    static OutputRequest::Base* readOutputRequest(Geometry::Mesh::Geometric& mesh, const json&);

    static OutputRequest::Domain readDomain(const json&);
    static Math::Axis::Local strToLocalAxes(const std::string& str);

    static bool checkVersionCompatibility(const std::string& version);

    static Geometry::Element::Group<> boxToElemGroup(
            Geometry::Mesh::Geometric& mesh,
            const std::string& line);

    static Geometry::Element::Group<Geometry::Nod> readCoordIdAsNodes(
            Geometry::Mesh::Geometric& mesh, const json&);

    static OutputRequest::Base::Type strToOutputType(std::string label);
    static Source::Generator::Type strToGeneratorType(std::string label);
    static Source::Generator::Hardness strToGeneratorHardness(std::string str);
    static Source::OnLine::Type strToNodalType(std::string label);

    static Source::OnLine::Hardness strToNodalHardness(std::string label);
    static Source::Port::TEM::ExcitationMode strToTEMMode(std::string);
    static Source::Port::Waveguide::ExcitationMode strToWaveguideMode(std::string);

    static PhysicalModel::PhysicalModel::Type strToMaterialType(std::string label);
    static PhysicalModel::Multiport::Multiport::Type strToMultiportType(std::string label);
    static PhysicalModel::Volume::Anisotropic::Model strToAnisotropicModel(std::string label);

    static Math::CVecI3 strToCVecI3(std::string str);
    static Math::CVecR3 strToCVecR3(std::string str);
    static Math::Constants::CartesianAxis strToCartesianAxis(std::string);

    static std::pair<Math::CVecR3, Math::CVecR3> strToBox(const std::string& str);
};

template<typename T>
Geometry::Element::Group<T> 
readElemIdsAsGroupOf(Geometry::Mesh::Geometric& mesh, const Parser::json& j) 
{
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
        const Parser::json& e) {
    Geometry::Element::Group<Geometry::ElemR> res;

    for (auto it = e.begin(); it != e.end(); ++it) {

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
