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

#ifndef SEMBA_PARSER_GID_PARSER_H_
#define SEMBA_PARSER_GID_PARSER_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "geometry/mesh/Unstructured.h"
#include "physicalModel/Multiport.h"
#include "physicalModel/SurfaceMultilayer.h"
#include "physicalModel/SurfaceSIBC.h"
#include "physicalModel/VolumeDispersive.h"
#include "physicalModel/VolumeAnisotropic.h"
#include "source/Dipole.h"
#include "source/Generator.h"
#include "source/OnLine.h"
#include "source/Planewave.h"
#include "source/port/Waveguide.h"
#include "source/port/TEM.h"
#include "Data.h"
#include "ProblemSize.h"

#include "parser/Parser.h"

#define LABEL_ENDING ':'

namespace SEMBA {
namespace Parser {
namespace GiD {

class Parser : public SEMBA::Parser::Parser, public FileSystem::Project {
public:
    Parser();
    Parser(const std::string& fn);
    virtual ~Parser();

    const ProblemSize* getProblemSize() const;

    virtual Data* read();
    void printInfo() const;
private:
    typedef enum {
        outRqOnPoint,
        outRqOnLine,
        outRqOnSurface,
        outRqOnVolume,
        bulkCurrentOnSurface,
        bulkCurrentOnVolume,
        farField
    } GiDOutputType;
    typedef enum {
        sibc,
        multilayer,
        undefinedSIBC
    } SIBCType;
    typedef enum {
        rectangular,
        undefined
    } WaveportShape;

    PhysicalModel::Group<>* physicalModels_;
    Geometry::Mesh::Unstructured* mesh_;
    ProblemSize pSize_;

    Solver::Options* readOptionsSolver();
    void readOptionsSolverOptions(Solver::Options&, const std::string&);
    //OptionsSolverFDTD* readOptionsSolverFDTD(const OptionsSolver& base);
    //OptionsSolverDGTD* readOptionsSolverDGTD(const OptionsSolver& base);
    //OptionsMesher* readMesherOptions();
    Source::Group<>* readEMSources();
    OutputRequest::Group<>* readOutputRequests();
    PhysicalModel::Group<>* readMaterials();
    Geometry::Mesh::Unstructured* readMesh();
    ProblemSize readProblemSize();
    PhysicalModel::VolumeDispersive* readDispersiveMatFile(
            const MatId id,
            const std::string& name,
            const FileSystem::Project& file) const;
    PhysicalModel::SurfaceSIBC* readIsotropicSurfMatFile(
            const MatId id,
            const std::string& name,
            const FileSystem::Project& file) const;
    PhysicalModel::SurfaceMultilayer*  readMultilayerSurf(
            const MatId id,
            const std::string& name,
            const std::string& layersString) const;
    Geometry::Layer::Group<> readLayers();
    Geometry::Coordinate::Group<Geometry::CoordR3> readCoordinates();
    Geometry::Element::Group<Geometry::ElemR> readElements(
            const Geometry::CoordR3Group&,
            const Geometry::Layer::Group<>&);
    void readHex8Elements (const Geometry::CoordR3Group& cG,
                           const Geometry::Layer::Group<>&,
                           std::vector<Geometry::ElemR*>& elems);
    void readTet10Elements(const Geometry::CoordR3Group& cG,
                           const Geometry::Layer::Group<>&,
                           std::vector<Geometry::ElemR*>& elems);
    void readTet4Elements (const Geometry::CoordR3Group& cG,
                           const Geometry::Layer::Group<>&,
                           std::vector<Geometry::ElemR*>& elems);
    void readTri6Elements (const Geometry::CoordR3Group& cG,
                           const Geometry::Layer::Group<>&,
                           std::vector<Geometry::ElemR*>& elems);
    void readTri3Elements (const Geometry::CoordR3Group& cG,
                           const Geometry::Layer::Group<>&,
                           std::vector<Geometry::ElemR*>& elems);
    void readLin2Elements (const Geometry::CoordR3Group& cG,
                           const Geometry::Layer::Group<>&,
                           std::vector<Geometry::ElemR*>& elems);
    Geometry::Grid3* readCartesianGrid();
    void readOutRqInstances(OutputRequest::Group<>* res);
    void getNextLabelAndValue(std::string& label, std::string& value);
    Source::PlaneWave* readPlaneWave();
    Source::Dipole* readDipole();
    Source::Port::Waveguide* readPortWaveguide();
    Source::Port::TEM* readPortTEM();
    Source::Generator* readGenerator();
    Source::OnLine* readSourceOnLine();
    PhysicalModel::PhysicalModel* readPhysicalModel(const MatId id);
    Source::Magnitude::Magnitude* readMagnitude(const std::string type);
    Math::Axis::Local strToLocalAxes(const std::string& str);

    std::string readVersion();
    bool checkVersionCompatibility(const std::string version) const;
    Geometry::Element::Group<> boxToElemGroup(const std::string& line);

    static OutputRequest::Base::Type strToOutputType(std::string label);
    static SIBCType strToSIBCType(std::string str);
    static GiDOutputType strToGidOutputType(std::string label);
    static OutputRequest::Domain strToDomain(std::string line);
    //static OptionsMesher::Mesher strToMesher(std::string);
    //static OptionsMesher::Mode strToMesherMode(std::string);
    static Math::CVecR3 strToCVecR3(const std::string& str);
    static Source::Generator::Type strToGeneratorType(std::string label);
    static Source::Generator::Hardness strToGeneratorHardness(std::string str);
    static Source::OnLine::Type strToNodalType(std::string label);
    static Source::OnLine::Hardness strToNodalHardness(std::string label);
    //static OptionsMesher::BoundType strToBoundType(std::string label);
    static PhysicalModel::PhysicalModel::Type strToMaterialType(
            std::string label);
    static PhysicalModel::Multiport::Type strToMultiportType(
            std::string label);
    static PhysicalModel::VolumeAnisotropic::Model strToAnisotropicModel(
            std::string label);

    //static OptionsSolverFDTD::PMLBacking strToPMLBacking(
    //        const std::string& string);
    //static OptionsSolverFDTD::NoNF2FF strToNoNF2FF(const std::string& string);
    //static OptionsSolver::Solver strToSolver(std::string);
    //static OptionsSolverFDTD::WireModel strToWireModel(std::string);
    //static OptionsSolverFDTD::InductanceModel strToInductanceModel(std::string);
    //static OptionsSolverFDTD::CompositeModel strToCompositeModel(std::string);
    static std::pair<Math::CVecR3, Math::CVecR3> strToBox(
            const std::string& str);
    static PhysicalModel::PoleResidue readPoleResiduePair(
            std::ifstream& stream);
};

} /* namespace GiD */
} /* namespace Parser */
} /* namespace SEMBA */

#endif /* SEMBA_PARSER_GID_PARSER_H_ */
