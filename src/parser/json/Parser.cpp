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

#include "../json/Parser.h"

#include "math/function/Gaussian.h"
#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Triangle6.h"
#include "geometry/element/Quadrilateral4.h"
#include "geometry/element/Tetrahedron4.h"
#include "geometry/element/Tetrahedron10.h"
#include "geometry/element/Hexahedron8.h"
#include "physicalModel/bound/Bound.h"
#include "physicalModel/predefined/PEC.h"
#include "physicalModel/predefined/PMC.h"
#include "physicalModel/predefined/SMA.h"
#include "physicalModel/multiport/Dispersive.h"
#include "physicalModel/multiport/Predefined.h"
#include "physicalModel/multiport/RLC.h"
#include "physicalModel/volume/AnisotropicCrystal.h"
#include "physicalModel/volume/AnisotropicFerrite.h"
#include "physicalModel/volume/Classic.h"
#include "physicalModel/volume/PML.h"
#include "physicalModel/wire/Wire.h"
#include "physicalModel/gap/Gap.h"
#include "source/port/WaveguideRectangular.h"
#include "source/port/TEMCoaxial.h"
#include "outputRequest/BulkCurrent.h"
#include "outputRequest/FarField.h"

namespace SEMBA {
namespace Parser {
namespace JSON {

Data* Parser::read(std::istream& stream) const {

    json j;
    try {
        stream >> j;
    }
    catch( const std::exception & ex ) {
        std::cerr << ex.what() << std::endl;
    }

    Util::ProgressBar progress;
    progress.init("Parser GiD-JSON", 7, 0);

    std::string version = j.at("_version").get<std::string>();
    if (!checkVersionCompatibility(version)) {
        throw std::logic_error(
                "File version " + version + " is not supported.");
    }
    progress.advance();

    Data* res = new Data();

    res->solver = readSolver( j );
    progress.advance();

    res->physicalModels = readPhysicalModels( j );
    progress.advance();

    res->mesh = readGeometricMesh( j );
    progress.advance();

//    res->sources = readSources( j );
//    progress.advance();
//
//    res->outputRequests = readOutputRequests( j );
//    progress.advance();
//
//    postReadOperations(res);
//    progress.advance();
//
//    progress.end();

    return res;
}

Solver::Info* Parser::readSolver(const json& j) {
    if (j.find("solverOptions") == j.end()) {
        return nullptr;
    }
    json solverOptions = j.at("solverOptions").get<json>();
    Solver::Settings opts = readSolverSettings(solverOptions);
    return new Solver::Info(
            solverOptions.at("solverName").get<std::string>(),
            std::move(opts));
}

Solver::Settings Parser::readSolverSettings(const json& j) {
    Solver::Settings opts;
    opts.setObject();
    for (json::const_iterator it = j.begin(); it != j.end(); ++it) {
        if (it->is_object()) {
            Solver::Settings aux = readSolverSettings(*it);
            opts.addMember(it.key(), std::move(aux));
        } else {
            Solver::Settings aux;
            std::stringstream value;
            value << *it;
            aux.setString(value.str());
            opts.addMember(it.key(), std::move(aux));
        }
    }
    return opts;
}

Geometry::Mesh::Geometric* Parser::readGeometricMesh(const json& j) {
    Geometry::Grid3 grid = readGrids(j);
    Geometry::Layer::Group<> layers = readLayers(j);
    Geometry::Coordinate::Group<Geometry::CoordR3> coords = readCoordinates(j);
    Geometry::Element::Group<Geometry::ElemR> elements =
            readElements(coords, layers, j);
    return new Geometry::Mesh::Geometric(grid, coords, elements, layers);
}

//Source::Group<>* Parser::readSources() {
//    Source::Group<>* res = new Source::Group<>();
//    bool finished = false;
//    bool found = false;
//    std::string label, value;
//    while (!found && !f_in.eof() ) {
//        getNextLabelAndValue(label, value);
//        if (label.compare("Excitations")==0) {
//            found = true;
//            finished = false;
//            // Runs over file reading elements excitation defined.
//            while (!finished && !f_in.eof() ) {
//                getNextLabelAndValue(label,value);
//                if (label.compare("Puntual excitation")==0) {
////                    res->add(readDipole());
//                } else if (label.compare("Planewave")==0) {
//                    res->add(readPlaneWave());
//                } else if (label.compare("Generator")==0) {
//                    res->add(readGenerator());
//                } else if (label.compare("Waveguide_port")==0) {
//                    res->add(readPortWaveguide());
//                } else if (label.compare("TEM_port")==0) {
//                    res->add(readPortTEM());
//                } else if (label.compare("Source_on_line")==0) {
//                    res->add(readSourceOnLine());
//                } else if (label.compare("End of Excitations")==0) {
//                    finished = true;
//                } // if: end of boundary conditions label was found.
//            } // while: loops until eof or the task is finished.
//        } // if: boundary condition label was found.
//    } // while: information was found or eof was reached.
//    if (!found) {
//        throw std::logic_error("Excitations label was not found.");
//    }
//    //
//    return res;
//}

PhysicalModel::Group<>* Parser::readPhysicalModels(const json& j){
    if (j.find("materials") == j.end()) {
        return nullptr;
    }
    PhysicalModel::Group<>* res = new PhysicalModel::Group<>();
    json materials = j.at("materials");
    for (json::const_iterator it = materials.begin();
            it != materials.end(); ++it) {
        res->add(readPhysicalModel( *it ));
    }
    return res;
}

PhysicalModel::PhysicalModel* Parser::readPhysicalModel(const json& mat) {
    PhysicalModel::PhysicalModel::Type type =
                strToMaterialType( mat.at("materialType").get<std::string>() );
    MatId id = MatId(mat.at("materialId").get<int>());
    std::string name = mat.at("name").get<std::string>();

    switch (type) {
    case PhysicalModel::PhysicalModel::PEC:
        return new PhysicalModel::Predefined::PEC (id, name);

    case PhysicalModel::PhysicalModel::PMC:
        return new PhysicalModel::Predefined::PMC(id, name);

    case PhysicalModel::PhysicalModel::SMA:
        return new PhysicalModel::Predefined::SMA(id, name);

    case PhysicalModel::PhysicalModel::PML:
        if (mat.at("automaticOrientation").get<bool>()) {
            return new PhysicalModel::Volume::PML(id, name, nullptr);
        } else {
            Math::Axis::Local* localAxes;
            localAxes = new Math::Axis::Local(
                    strToLocalAxes(mat.at("localAxes").get<std::string>()));
            return new PhysicalModel::Volume::PML(id, name, localAxes);
        }

    case PhysicalModel::PhysicalModel::classic:
        return new PhysicalModel::Volume::Classic(id, name,
                mat.at("permittivity").get<double>(),
                mat.at("permeability").get<double>(),
                mat.at("electricConductivity").get<double>(),
                mat.at("magneticConductivity").get<double>());

    case PhysicalModel::PhysicalModel::elecDispersive:
        return new PhysicalModel::Volume::Dispersive(id, name,
                mat.at("filename").get<std::string>());

    case PhysicalModel::PhysicalModel::wire:
    {
        std::string wireType = mat.at("wireType").get<std::string>();
        if (wireType.compare("Dispersive") == 0) {
            return new PhysicalModel::Wire::Wire(id, name,
                    mat.at("radius").get<double>(),
                    mat.at("filename").get<std::string>());
        } else if(wireType.compare("SeriesParallel") == 0) {
            return new PhysicalModel::Wire::Wire(id, name,
                    mat.at("radius").get<double>(),
                    mat.at("resistance").get<double>(),
                    mat.at("inductance").get<double>(),
                    mat.at("capacitance").get<double>(),
                    mat.at("parallelResistance").get<double>(),
                    mat.at("parallelInductance").get<double>(),
                    mat.at("parallelCapacitance").get<double>());
        } else if(wireType.compare("Standard") == 0) {
            return new PhysicalModel::Wire::Wire(id, name,
                    mat.at("resistance"),
                    mat.at("inductance"));
        } else {
            throw std::logic_error("Unrecognized wire type" + wireType);
        }
    }

    case PhysicalModel::PhysicalModel::anisotropic:
    {
        std::string str = mat.at("anisotropicModel").get<std::string>();
        if (str.compare("Crystal")==0) {
            return new PhysicalModel::Volume::AnisotropicCrystal(id, name,
                    strToLocalAxes(mat.at("localAxes").get<std::string>()),
                    strToCVecR3(
                            mat.at("relativePermittiviy").get<std::string>()),
                    mat.at("crystalRelativePermeability").get<double>());
        } else if (str.compare("Ferrite")==0) {
            return new PhysicalModel::Volume::AnisotropicFerrite(id, name,
                    strToLocalAxes(mat.at("localAxes").get<std::string>()),
                    mat.at("kappa").get<double>(),
                    mat.at("ferriteRelativePermeability").get<double>(),
                    mat.at("ferriteRelativePermittivity").get<double>());
        } else {
            throw std::logic_error("Unrecognized Anisotropic Model: " + str);
        }
    }

    case PhysicalModel::PhysicalModel::isotropicsibc:
    {
        std::string sibcType = mat.at("surfaceType").get<std::string>();
        if (sibcType.compare("File")==0) {
            return new PhysicalModel::Surface::SIBC(id, name,
                    mat.at("filename").get<std::string>() );
        } else if (sibcType.compare("Layers")==0) {
            return readMultilayerSurface(id, name,
                    mat.at("layers").get<json>());
        } else {
            throw std::logic_error("Unrecognized SIBC type: " + sibcType);
        }
    }

    case PhysicalModel::PhysicalModel::gap:
        return new PhysicalModel::Gap::Gap(id, name,
                mat.at("width").get<double>());

    case PhysicalModel::PhysicalModel::multiport:
    {
        PhysicalModel::Multiport::Multiport::Type mpType =
                strToMultiportType(mat.at("connectorType").get<std::string>());
        if (mpType == PhysicalModel::Multiport::Multiport::shortCircuit) {
            return new PhysicalModel::Multiport::Predefined(id, name, mpType);
        } else if (mpType == PhysicalModel::Multiport::Multiport::dispersive) {
            return new PhysicalModel::Multiport::Dispersive(id, name,
                    mat.at("filename").get<std::string>());
        } else {
            return new PhysicalModel::Multiport::RLC(id, name, mpType,
                    mat.at("resistance").get<double>(),
                    mat.at("inductance").get<double>(),
                    mat.at("capacitance").get<double>());
        }
    }

    default:
        throw std::logic_error("Material type not recognized for: " + name);
    }
}

//OutputRequest::Group<>* Parser::readOutputRequests() {
//    OutputRequest::Group<>* res = new OutputRequest::Group<>();
//    bool finished;
//    bool found = false;
//    std::string line, label, value;
//    while (!found && !f_in.eof() ) {
//        getNextLabelAndValue(label, value);
//        if (label.compare("Output Requests")==0) {
//            found = true;
//            finished = false;
//            while (!finished && !f_in.eof() ) {
//                getNextLabelAndValue(label, value);
//                if (label.compare("Output request instance")==0) {
//                    readOutRqInstances(res);
//                } else if (label.compare("End of Output Requests")==0) {
//                    finished = true;
//                }
//            }
//        }
//    }
//    return res;
//}
//
//Geometry::Element::Group<> Parser::boxToElemGroup(const std::string& line) {
//    Geometry::BoxR3 box = strToBox(line);
//    if (box.isVolume()) {
//        Geometry::HexR8* hex =
//            new Geometry::HexR8(*mesh_, Geometry::ElemId(0), box);
//        mesh_->elems().addId(hex);
//        Geometry::Element::Group<> elems(hex);
//        return elems;
//    } else if (box.isSurface()) {
//        Geometry::QuaR4* qua =
//            new Geometry::QuaR4(*mesh_, Geometry::ElemId(0), box);
//        mesh_->elems().addId(qua);
//        Geometry::Element::Group<> elems(qua);
//        return elems;
//    } else {
//        throw std::logic_error(
//                "Box to Elem Group only works for volumes and surfaces");
//    }
//}
//
//void Parser::readOutRqInstances(OutputRequest::Group<>* res) {
//    bool finished = false;
//    OutputType outputType;
//    while (!finished && !f_in.eof()) {
//        std::string line, label, value;
//        getNextLabelAndValue(label,value);
//        if (label.compare("GiDOutputType")==0) {
//            outputType = strToGidOutputType(trim(value));
//        } else if (label.compare("Number of elements")==0) {
//            std::size_t nE = atoi(value.c_str());
//            for (std::size_t i = 0; i < nE; i++) {
//                getNextLabelAndValue(label,value);
//                std::string name = trim(value);
//                getNextLabelAndValue(label,value);
//                OutputRequest::Base::Type type = strToOutputType(trim(value));
//                getNextLabelAndValue(label,value);
//                OutputRequest::Domain domain = strToDomain(value);
//                switch (outputType) {
//                case Parser::outRqOnPoint:
//                {
//                    getNextLabelAndValue(label,value);
//                    Geometry::CoordId coordId(atoi(value.c_str()));
//                    const Geometry::CoordR3* coord =
//                        mesh_->coords().getId(coordId);
//                    Geometry::NodR* node =
//                        new Geometry::NodR(Geometry::ElemId(0), &coord);
//                    mesh_->elems().addId(node);
//                    Geometry::Element::Group<Geometry::Nod> elems(node);
//                    res->add(
//                        new OutputRequest::OutputRequest<Geometry::Nod>(
//                            domain, type, name, elems));
//                    break;
//                }
//                case Parser::outRqOnLine:
//                {
//                    getNextLabelAndValue(label,value);
//                    std::vector<Geometry::ElemId> ids;
//                    ids.push_back(Geometry::ElemId(atoi(value.c_str())));
//                    Geometry::Element::Group<Geometry::ElemR> elems =
//                        mesh_->elems().getId(ids);
//                    Geometry::Element::Group<Geometry::Lin> lines =
//                        elems.getOf<Geometry::Lin>();
//                    res->add(new OutRqLine(domain, type, name, lines));
//                    break;
//                }
//                case Parser::outRqOnSurface:
//                {
//                    getNextLabelAndValue(label,value);
//                    std::vector<Geometry::ElemId> ids;
//                    ids.push_back(Geometry::ElemId(atoi(value.c_str())));
//                    Geometry::Element::Group<Geometry::ElemR> elems =
//                            mesh_->elems().getId(ids);
//                    Geometry::Element::Group<Geometry::Surf> surfs =
//                            elems.getOf<Geometry::Surf>();
//                    res->add(new OutRqSurface(domain, type, name, surfs));
//                    break;
//                }
//                case Parser::outRqOnLayer:
//                {
//                    getline_(line);
//                    Geometry::Element::Group<> elems = boxToElemGroup(line);
//                    if (elems.getOf<Geometry::Vol>().size()) {
//                        res->add(
//                            new OutputRequest::OutputRequest<Geometry::Vol>(
//                                domain, type, name,
//                                elems.getOf<Geometry::Vol>()));
//                    } else if (elems.getOf<Geometry::Surf>().size()) {
//                        res->add(
//                            new OutputRequest::OutputRequest<Geometry::Surf>(
//                                domain, type, name,
//                                elems.getOf<Geometry::Surf>()));
//                    } else {
//                        throw std::logic_error("Layer for OutRq on Layer must"
//                                               " be volume or surface");
//                    }
//                    break;
//                }
//                case Parser::bulkCurrentOnSurface:
//                {
//                    Math::Constants::CartesianAxis dir;
//                    std::size_t skip;
//                    getNextLabelAndValue(label,value);
//                    switch (value[0]) {
//                    case 'x':
//                        dir = Math::Constants::x;
//                        break;
//                    case 'y':
//                        dir = Math::Constants::y;
//                        break;
//                    case 'z':
//                        dir = Math::Constants::z;
//                        break;
//                    default:
//                        dir = Math::Constants::x;
//                    }
//                    getNextLabelAndValue(label,value);
//                    skip = atoi(value.c_str());
//                    getline_(line);
//                    std::vector<Geometry::ElemId> ids;
//                    ids.push_back(Geometry::ElemId(atoi(line.c_str())));
//                    Geometry::Element::Group<Geometry::ElemR> elems =
//                        mesh_->elems().getId(ids);
//                    Geometry::Element::Group<Geometry::Surf> surfs =
//                        elems.getOf<Geometry::Surf>();
//                    res->add(
//                        new OutputRequest::BulkCurrent(
//                            domain, name, surfs, dir, skip));
//                    break;
//                }
//                case Parser::bulkCurrentOnLayer:
//                {
//                    Math::Constants::CartesianAxis dir;
//                    std::size_t skip;
//                    getNextLabelAndValue(label,value);
//                    switch (value[0]) {
//                    case 'x':
//                        dir = Math::Constants::x;
//                        break;
//                    case 'y':
//                        dir = Math::Constants::y;
//                        break;
//                    case 'z':
//                        dir = Math::Constants::z;
//                        break;
//                    default:
//                        dir = Math::Constants::x;
//                    }
//                    getNextLabelAndValue(label,value);
//                    skip = atoi(value.c_str());
//                    getline_(line);
//                    Geometry::Element::Group<Geometry::Vol> elems =
//                        boxToElemGroup(line);
//                    res->add(
//                        new OutputRequest::BulkCurrent(
//                            domain, name, elems, dir, skip));
//                    break;
//                }
//                case Parser::farField:
//                {
//                    getline_(line);
//                    Geometry::Element::Group<Geometry::Vol> elems =
//                        boxToElemGroup(line);
//                    Math::Real iThDeg, fThDeg, sThDeg, iPhiDeg, fPhiDeg, sPhiDeg;
//                    f_in >> iThDeg >> fThDeg >> sThDeg
//                        >> iPhiDeg >> fPhiDeg >> sPhiDeg;
//                    getline_(line);
//                    Math::Real iThRad, fThRad, sThRad, iPhiRad, fPhiRad, sPhiRad;
//                    static const Math::Real degToRad = 2.0 * Math::Constants::pi / 360.0;
//                    iThRad = iThDeg * degToRad;
//                    fThRad = fThDeg * degToRad;
//                    sThRad = sThDeg * degToRad;
//                    iPhiRad = iPhiDeg * degToRad;
//                    fPhiRad = fPhiDeg * degToRad;
//                    sPhiRad = sPhiDeg * degToRad;
//                    OutputRequest::FarField* oRFF =
//                        new OutputRequest::FarField(
//                            domain, name, elems,
//                            iThRad, fThRad, sThRad, iPhiRad, fPhiRad, sPhiRad);
//                    res->add(oRFF);
//                    break;
//                }
//                default:
//                    throw std::logic_error(
//                            "Unrecognized GiD Output request type:" + type);
//                }
//            } // End of loop running over the elements.
//        } else if (label.compare("End of Output request instance")==0) {
//            finished = true;
//        } else {
//            throw std::logic_error("Label not identified: " + label);
//        } // End of condition comparing labels.
//    }
//}

Geometry::Layer::Group<> Parser::readLayers(const json& j) {
    if (j.find("layers") == j.end()) {
        throw std::logic_error("layers object was not found.");
    }

    Geometry::Layer::Group<> res;
    const json layers = j.at("layers");
    for (json::const_iterator it = layers.begin(); it != layers.end(); ++it) {
        res.add(new Geometry::Layer::Layer(
                Geometry::Layer::Id(it->at("id").get<int>()),
                it->at("name").get<std::string>()));
    }
    return res;
}

Geometry::Coordinate::Group<Geometry::CoordR3> Parser::readCoordinates(
        const json& j) {

    if (j.find("coordinates") == j.end()) {
        throw std::logic_error("Coordinates label was not found.");
    }

    Geometry::Coordinate::Group<Geometry::CoordR3> res;
    const json& c = j.at("coordinates").get<json>();
    for (json::const_iterator it = c.begin(); it != c.end(); ++it) {
        Geometry::CoordId id;
        Math::CVecR3 pos;
        std::stringstream ss(it->get<std::string>());
        ss >> id >> pos(0) >> pos(1) >> pos(2);
        res.add(new Geometry::CoordR3(id, pos));
    }
    return res;
}

Geometry::Element::Group<Geometry::ElemR> Parser::readElements(
        const PhysicalModel::Group<>& mG,
        const Geometry::Layer::Group<>& lG,
        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
        const json& j) {

    if (j.find("elements") == j.end()) {
        throw std::logic_error("Elements label was not found.");
    }

    Geometry::Element::Group<Geometry::ElemR> res;
    const json& elems = j.at("elements").get<json>();

    const json& hex = elems.at("hexahedra").get<json>();
    for (json::const_iterator it = hex.begin(); it != hex.end(); ++it) {
        ParsedElementIds elemIds = readElementIds(it->get<std::string>(), 8);
        ParsedElementPtrs elemPtrs =
                convertElementIdsToPtrs(elemIds, lG, mG, cG);
        res.add(new Geometry::HexR8(elemIds.elemId,
                elemPtrs.vPtr.data(), elemPtrs.layerPtr, elemPtrs.matPtr));
    }

//        if (label.compare("Linear Hexahedral Elements")==0 ||
//                label.compare("Hexahedral Elements")==0) {
//            readHex8Elements(cG, lG, elems);
//        } else if (label.compare("Quadratic Tetrahedral Elements")==0) {
//            readTet10Elements(cG, lG, elems);
//        } else if (label.compare("Linear Tetrahedral Elements")==0 ||
//                   label.compare("Tetrahedral Elements")==0) {
//            readTet4Elements(cG, lG, elems);
//        } else if(label.compare("Quadrilateral Elements")==0) {
//        	readQua4Elements(cG, lG, elems);
//        } else if (label.compare("Quadratic Triangle Elements")==0) {
//            readTri6Elements(cG, lG, elems);
//        } else if (label.compare("Linear Triangle Elements")==0 ||
//                   label.compare("Triangle Elements")==0) {
//            readTri3Elements(cG, lG, elems);
//        } else if (label.compare("Linear Line Elements")==0 ||
//                   label.compare("Line Elements")==0) {
//            readLin2Elements(cG, lG, elems);
//        } else if(label.find("End of Elements") != label.npos) {
//            finished = true;
//        }
//    }
//    // Shows error message if the elements label was not found.
//    if (!found) {
//        throw std::logic_error("\"Elements\" label was not found.");
//    }
//    // This code is reached only in case of "End of elements" is not found.
//    if (!finished) {
//        throw std::logic_error("\"End of elements\" label was not found.");
//    }
//    //
}
//
//void Parser::readHex8Elements(
//        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
//        const Geometry::Layer::Group<>& lG,
//        Geometry::Element::Group<Geometry::ElemR>& elems) {
//    Geometry::ElemId id;
//    Geometry::CoordId vId;
//    const Geometry::CoordR3* v[8];
//    MatId matId;
//    for (std::size_t i = 0; i < pSize_.hex8; i++) {
//        f_in >> id;
//        for (std::size_t j = 0; j < 8; j++) {
//            f_in >> vId;
//            v[j] = cG.getId(vId);
//        }
//        f_in >> matId;
//        progress_.advance();
//        const PhysicalModel::PhysicalModel* mat;
//        if (matId != MatId(0)) {
//            mat = physicalModels_->getId(matId);
//        } else {
//            mat = nullptr;
//        }
//        elems.add(new Geometry::HexR8(id, v, nullptr, mat));
//    }
//}
//void Parser::readTet4Elements(
//        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
//        const Geometry::Layer::Group<>& lG,
//        Geometry::Element::Group<Geometry::ElemR>& elems) {
//    Geometry::ElemId id;
//    Geometry::CoordId vId;
//    const Geometry::CoordR3* v[4];
//    Geometry::Layer::Id layerId;
//    MatId matId;
//    for (std::size_t i = 0; i < pSize_.tet4; i++) {
//        f_in >> id;
//        for (std::size_t j = 0; j < 4; j++) {
//            f_in >> vId;
//            v[j] = cG.getId(vId);
//        }
//        f_in >> matId >> layerId;
//        progress_.advance();
//        const Geometry::Layer::Layer* lay;
//        if (layerId != Geometry::LayerId(0)) {
//            lay = lG.getId(layerId);
//        }
//        else {
//            lay = nullptr;
//        }
//        const PhysicalModel::PhysicalModel* mat;
//        if (matId != MatId(0)) {
//            mat = physicalModels_->getId(matId);
//        }
//        else {
//            mat = nullptr;
//        }
//        elems.add(new Geometry::Tet4(id, v, lay, mat));
//    }
//}
//void Parser::readQua4Elements(
//        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
//        const Geometry::Layer::Group<>& lG,
//        Geometry::Element::Group<Geometry::ElemR>& elems) {
//    Geometry::ElemId id;
//    Geometry::CoordId vId;
//    const Geometry::CoordR3* v[4];
//    Geometry::Layer::Id layerId;
//    MatId matId;
//    Math::CVecR3 normal;
//    for (std::size_t i = 0; i < pSize_.tri6; i++) {
//        f_in >> id;
//        for (std::size_t j = 0; j < 4; j++) {
//            f_in >> vId;
//            v[j] = cG.getId(vId);
//        }
//        f_in >> matId >> layerId;
//        progress_.advance();
//        const Geometry::Layer::Layer* lay;
//               if (layerId != Geometry::LayerId(0)) {
//                   lay = lG.getId(layerId);
//               }
//               else {
//                   lay = nullptr;
//               }
//        const PhysicalModel::PhysicalModel* mat;
//        if (matId != MatId(0)) {
//            mat = physicalModels_->getId(matId);
//        }
//        else {
//            mat = nullptr;
//        }
//        elems.add(new Geometry::QuaR4(id, v, lay, mat));
//    }
//}
//
//void Parser::readTri6Elements(
//        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
//        const Geometry::Layer::Group<>& lG,
//        Geometry::Element::Group<Geometry::ElemR>& elems) {
//    Geometry::ElemId id;
//    Geometry::CoordId vId;
//    const Geometry::CoordR3* v[6];
//    MatId matId;
//    Math::CVecR3 normal;
//    for (std::size_t i = 0; i < pSize_.tri6; i++) {
//        f_in >> id;
//        for (std::size_t j = 0; j < 6; j++) {
//            f_in >> vId;
//            v[j] = cG.getId(vId);
//        }
//        f_in >> matId;
//        progress_.advance();
//        const PhysicalModel::PhysicalModel* mat;
//        if (matId != MatId(0)) {
//            mat = physicalModels_->getId(matId);
//        }
//        else {
//            mat = nullptr;
//        }
//        elems.add(new Geometry::Tri6(id, v, nullptr, mat));
//    }
//}
//
//void Parser::readTri3Elements(
//        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
//        const Geometry::Layer::Group<>& lG,
//        Geometry::Element::Group<Geometry::ElemR>& elems) {
//    Geometry::ElemId id;
//    Geometry::CoordId vId;
//    const Geometry::CoordR3* v[3];
//    Geometry::Layer::Id layerId;
//    MatId matId;
//    Math::CVecR3 normal;
//    for (std::size_t i = 0; i < pSize_.tri3; i++) {
//        f_in >> id;
//        for (std::size_t j = 0; j < 3; j++) {
//            f_in >> vId;
//            v[j] = cG.getId(vId);
//        }
//        f_in >> matId >> layerId;
//        progress_.advance();
//        const Geometry::Layer::Layer* lay;
//        if (layerId != Geometry::LayerId(0)) {
//            lay = lG.getId(layerId);
//        }
//        else {
//            lay = nullptr;
//        }
//        const PhysicalModel::PhysicalModel* mat;
//        if (matId != MatId(0)) {
//            mat = physicalModels_->getId(matId);
//        }
//        else {
//            mat = nullptr;
//        }
//        elems.add(new Geometry::Tri3(id, v, lay, mat));
//    }
//}
//
//void Parser::readLin2Elements(
//        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
//        const Geometry::Layer::Group<>& lG,
//        Geometry::Element::Group<Geometry::ElemR>& elems) {
//    Geometry::ElemId id;
//    Geometry::CoordId vId;
//    const Geometry::CoordR3* v[2];
//    Geometry::Layer::Id layerId;
//    MatId matId;
//    for (std::size_t i = 0; i < pSize_.lin2; i++) {
//        f_in >> id;
//        for (std::size_t j = 0; j < 2; j++) {
//            f_in >> vId;
//            v[j] = cG.getId(vId);
//        }
//        f_in >> matId >> layerId;
//        progress_.advance();
//        const Geometry::Layer::Layer* lay;
//        if (layerId != Geometry::LayerId(0)) {
//            lay = lG.getId(layerId);
//        }
//        else {
//            lay = nullptr;
//        }
//        const PhysicalModel::PhysicalModel* mat;
//        if (matId != MatId(0)) {
//            mat = physicalModels_->getId(matId);
//        }
//        else {
//            mat = nullptr;
//        }
//        elems.add(new Geometry::LinR2(id, v, lay, mat));
//    }
//}
//

PhysicalModel::Surface::Multilayer* Parser::readMultilayerSurface(
        const MatId id,
        const std::string& name,
        const json& layers) const {
    std::vector<Math::Real> thick, relEp, relMu, eCond, mCond;
    for (json::const_iterator it = layers.begin(); it != layers.end(); ++it) {
        thick.push_back( it->at("thickness").get<double>() );
        relEp.push_back( it->at("permittivity").get<double>() );
        relMu.push_back( it->at("permeability").get<double>() );
        eCond.push_back( it->at("elecCond").get<double>() );
        mCond.push_back( it->at("magnCond").get<double>() );
    }
    return new PhysicalModel::Surface::Multilayer(
            id, name, thick, relEp, relMu, eCond, mCond);
}

Geometry::Grid3 Parser::readGrids(const json& j) {
    if (j.find("grids") == j.end()) {
        throw std::logic_error("Grids object not found.");
    }

    json g = j.at("grids").front();
    std::string gridType = g.at("gridType").get<std::string>();
    if (gridType.compare("gridCondition") == 0) {
        // Initializes basic grid.
        Geometry::Grid3 res;
        if (g.at("type").get<std::string>().compare("by_number_of_cells") == 0) {
            res = Geometry::Grid3(
                    strToBox(g.at("layerBox").get<std::string>()),
                    strToCVecI3(g.at("directions").get<std::string>()));
        } else {
            res = Geometry::Grid3(
                    strToBox(g.at("layerBox").get<std::string>()),
                    strToCVecR3(g.at("directions").get<std::string>()));
        }

        // Applies boundary padding operations.
        std::pair<Math::CVecR3, Math::CVecR3> boundaryMeshSize(
                strToCVecR3(g.at("lowerPaddingMeshSize").get<std::string>()),
                strToCVecR3(g.at("upperPaddingMeshSize").get<std::string>()));
        std::pair<Math::CVecR3, Math::CVecR3> boundaryPadding(
                strToCVecR3(g.at("lowerPadding").get<std::string>()),
                strToCVecR3(g.at("upperPadding").get<std::string>()));
        if (g.at("boundaryPaddingType").get<std::string>().compare(
                "by_number_of_cells") == 0) {
            boundaryPadding.first  *= boundaryMeshSize.first;
            boundaryPadding.second *= boundaryMeshSize.second;
        }
        res.enlarge(boundaryPadding, boundaryMeshSize);
        return res;

    } else if (gridType.compare("nativeGiD") == 0) {
        std::vector<Math::Real> pos[3];
        pos[0] = g.at("xCoordinates").get<std::vector<double>>();
        pos[1] = g.at("yCoordinates").get<std::vector<double>>();
        pos[2] = g.at("zCoordinates").get<std::vector<double>>();
        return Geometry::Grid3(pos);

    } else {
        throw std::logic_error("Unrecognized grid type: " + gridType);
    }
}
//
//Source::PlaneWave* Parser::readPlaneWave() {
//    std::string filename;
//    std::string label, value;
//    DefinitionMode definitionMode;
//    Math::CVecR3 dir, pol;
//    std::pair<Math::Real,Math::Real> directionAngles, polarizationAngles;
//    Math::Int numberOfRandomPlanewaves;
//    Math::Real relativeVariationOfRandomDelay;
//    static const Math::Real degToRad = 2.0 * Math::Constants::pi / 360.0;
//    Geometry::Element::Group<Geometry::Vol> elems;
//    Source::Magnitude::Magnitude* mag;
//    while(!f_in.eof()) {
//        getNextLabelAndValue(label, value);
//        if (label.compare("Definition mode")==0) {
//            definitionMode = strToDefinitionMode(value);
//        } else if (label.compare("Direction vector")==0) {
//            dir = strToCartesianVector(value);
//        } else if (label.compare("Polarization vector") == 0) {
//            pol = strToCartesianVector(value);
//        } else if (label.compare("Direction theta")==0) {
//            directionAngles.first = atof(value.c_str()) * degToRad;
//        } else if (label.compare("Direction phi")==0) {
//            directionAngles.second = atof(value.c_str()) * degToRad;
//        } else if (label.compare("Polarization alpha")==0) {
//            polarizationAngles.first = atof(value.c_str()) * degToRad;
//        } else if (label.compare("Polarization beta")==0) {
//            polarizationAngles.second = atof(value.c_str()) * degToRad;
//        } else if (label.compare("Number of random planewaves")==0) {
//            numberOfRandomPlanewaves = atoi(value.c_str());
//        } else if (label.compare("Relative variation of random delay")==0) {
//            relativeVariationOfRandomDelay = atof(value.c_str());
//        } else if (label.compare("Excitation") == 0) {
//            mag = readMagnitude(value);
//        } else if (label.compare("Layer Box") == 0) {
//            elems = boxToElemGroup(value);
//        } else if (label.compare("Number of elements")==0) {
//            std::size_t nE = atoi(value.c_str());
//            elems.clear();
//            elems.reserve(nE);
//            for (std::size_t i = 0; i < nE; i++) {
//                Geometry::ElemId id;
//                f_in >> id;
//                progress_.advance();
//                elems.add(mesh_->elems().getId(id));
//            }
//        } else if (label.compare("End of Planewave")==0) {
//            switch (definitionMode) {
//            case DefinitionMode::byVectors:
//                return new Source::PlaneWave(mag, elems, dir, pol);
//            case DefinitionMode::byAngles:
//                return new Source::PlaneWave(mag, elems, directionAngles,
//                        polarizationAngles);
//            case DefinitionMode::randomizedMultisource:
//                return new Source::PlaneWave(mag, elems,
//                        numberOfRandomPlanewaves,
//                        relativeVariationOfRandomDelay);
//            }
//
//        }
//    }
//    throw std::logic_error("End of Planewave label not found.");
//}
//
//Source::Port::Waveguide* Parser::readPortWaveguide() {
//    std::size_t numElements = 0;
//    Source::Magnitude::Magnitude* mag;
//    WaveportShape shape = WaveportShape::rectangular;
//    Source::Port::Waveguide::ExcitationMode excitationMode =
//        Source::Port::Waveguide::TE;
//    std::pair<Math::UInt,Math::UInt> mode(1,0);
//    std::string line, label, value;
//    Geometry::Element::Group<const Geometry::Surf> surfs;
//    bool finished = false;
//    while (!finished && !f_in.eof()) {
//        getNextLabelAndValue(label,value);
//        if (!label.compare("Shape")) {
//            if (value.find("Rectangular") != value.npos) {
//                shape = WaveportShape::rectangular;
//            } else {
//                throw std::logic_error("Unreckognized waveguide port shape.");
//            }
//        } else if (label.compare("Excitation") == 0) {
//            mag = readMagnitude(value);
//        } else if (label.compare("ExcitationMode") == 0) {
//            if (value.find("TE") != value.npos) {
//                excitationMode = Source::Port::Waveguide::TE;
//            } else if (value.find("TM") != value.npos) {
//                excitationMode = Source::Port::Waveguide::TM;
//            }
//        } else if (label.compare("FirstMode") == 0) {
//            mode.first = atoi(value.c_str());
//        } else if (label.compare("SecondMode") == 0) {
//            mode.second = atoi(value.c_str());
//        } else if (label.compare("Number of elements") == 0) {
//            numElements = atoi(value.c_str());
//        } else if (label.compare("Elements") == 0) {
//            std::size_t e, f;
//            std::vector<Geometry::Element::Face> faces;
//            for (std::size_t i = 0; i < numElements; i++) {
//                f_in >> e >> f;
//                progress_.advance();
//                const Geometry::ElemR* elem =
//                    mesh_->elems().getId(Geometry::ElemId(e));
//                if (elem->is<Geometry::VolR>()) {
//                    const Geometry::VolR* vol =
//                        elem->castTo<Geometry::VolR>();
//                    faces.push_back(Geometry::Element::Face(vol, f - 1));
//                } else if (elem->is<Geometry::SurfR>()) {
//                    surfs.add(elem->castTo<Geometry::SurfR>());
//                }
//            }
//            surfs.add(mesh_->getSurfsMatching(faces));
//            if (surfs.size() < faces.size()) {
//                surfs.printInfo();
//                throw std::logic_error("Could not find surfaces matching element faces.");
//            }
//            if (surfs.size() == 0) {
//                throw std::logic_error("No surfaces read on waveguide port.");
//            }
//        } else if (label.find("End of Waveguide port") != label.npos) {
//            finished = true;
//        }
//        if (f_in.eof()) {
//            throw std::logic_error("End of Waveguide port not found");
//        }
//    }
//    // Throws error message if finished was not updated.
//    if (!finished) {
//        throw std::logic_error("End of excitation type label not found.");
//    }
//
//    Source::Port::Bound3 boundTerminations;
//    // X bounds
//    if (settings_("Mesher options")("Lower x bound").isString()) {
//        boundTerminations[0][0] = strToBoundType(
//                settings_("Mesher options")("Lower x bound").getString());
//    } else {
//        boundTerminations[0][0] = new PhysicalModel::Bound::PML(MatId(0));
//    }
//    if (settings_("Mesher options")("Upper x bound").isString()) {
//        boundTerminations[0][1] = strToBoundType(
//                    settings_("Mesher options")("Upper x bound").getString());
//    } else {
//        boundTerminations[0][1] = new PhysicalModel::Bound::PML(MatId(0));
//    }
//    // Y bounds
//    if (settings_("Mesher options")("Lower y bound").isString()) {
//        boundTerminations[1][0] = strToBoundType(
//                    settings_("Mesher options")("Lower y bound").getString());
//    } else {
//        boundTerminations[1][0] = new PhysicalModel::Bound::PML(MatId(0));
//    }
//    if (settings_("Mesher options")("Upper y bound").isString()) {
//        boundTerminations[1][1] = strToBoundType(
//                    settings_("Mesher options")("Upper y bound").getString());
//    } else {
//        boundTerminations[1][1] = new PhysicalModel::Bound::PML(MatId(0));
//    }
//    // Z bounds
//    if (settings_("Mesher options")("Lower z bound").isString()) {
//        boundTerminations[2][0] = strToBoundType(
//                    settings_("Mesher options")("Lower z bound").getString());
//    } else {
//        boundTerminations[2][0] = new PhysicalModel::Bound::PML(MatId(0));
//    }
//    if (settings_("Mesher options")("Upper z bound").isString()) {
//        boundTerminations[2][1] = strToBoundType(
//                    settings_("Mesher options")("Upper z bound").getString());
//    } else {
//        boundTerminations[2][1] = new PhysicalModel::Bound::PML(MatId(0));
//    }
//
//    // Waveport shape.
//    if (shape == WaveportShape::rectangular) {
//        return new Source::Port::WaveguideRectangular(mag, surfs,
//                excitationMode, mode, boundTerminations);
//    } else {
//        throw std::logic_error("Unsupported Waveport shape.");
//    }
//}
//
//Source::Port::TEM* Parser::readPortTEM() {
//    std::size_t numElements = 0;
//    Source::Magnitude::Magnitude* mag;
//    Source::Port::TEM::ExcitationMode excitationMode = Source::Port::TEM::voltage;
//    Math::CVecR3 origin;
//    Math::Real innerRadius, outerRadius;
//    std::string line, label, value;
//    Geometry::Element::Group<const Geometry::Surf> surfs;
//    bool finished = false;
//    while (!finished && !f_in.eof()) {
//        getNextLabelAndValue(label,value);
//        if (label.compare("Excitation") == 0) {
//            mag = readMagnitude(value);
//        } else if (!label.compare("ExcitationMode")) {
//            if (value.find("Voltage") != value.npos) {
//                excitationMode = Source::Port::TEM::voltage;
//            } else if (value.find("Current") != value.npos) {
//                excitationMode = Source::Port::TEM::current;
//            }
//        } else if (!label.compare("Origin")) {
//            origin = strToCVecR3(value);
//        } else if (!label.compare("Inner radius")) {
//            innerRadius = atof(value.c_str());
//        } else if (!label.compare("Outer radius")) {
//            outerRadius = atof(value.c_str());
//        } else if (!label.compare("Number of elements")) {
//            numElements = atoi(value.c_str());
//        } else if (!label.compare("Elements")) {
//            std::size_t e, f;
//            std::vector<Geometry::Element::Face> faces;
//            for (std::size_t i = 0; i < numElements; i++) {
//                f_in >> e >> f;
//                progress_.advance();
//                const Geometry::ElemR* elem =
//                    mesh_->elems().getId(Geometry::ElemId(e));
//                if (elem->is<Geometry::VolR>()) {
//                    const Geometry::VolR* vol = elem->castTo<Geometry::VolR>();
//                    faces.push_back(Geometry::Element::Face(vol,f-1));
//                } else if (elem->is<Geometry::SurfR>()) {
//                    surfs.add(elem->castTo<Geometry::SurfR>());
//                }
//            }
//            surfs.add(mesh_->getSurfsMatching(faces));
//            if (surfs.size() < faces.size()) {
//                surfs.printInfo();
//                throw std::logic_error("Could not find surfaces matching element faces.");
//            }
//            if (surfs.size() == 0) {
//                throw std::logic_error("No surfaces read on TEM port.");
//            }
//        } else if (label.find("End of TEM port") != label.npos) {
//            finished = true;
//        }
//        if (f_in.eof()) {
//            throw std::logic_error("End of TEM port not found");
//        }
//    }
//    // Throws error message if finished was not updated.
//    if (!finished) {
//        throw std::logic_error("End of excitation type label not found.");
//    }
//    return new Source::Port::TEMCoaxial(mag, surfs, excitationMode, origin,
//            innerRadius, outerRadius);
//}
//
//Source::Generator* Parser::readGenerator() {
//    Geometry::Element::Group<Geometry::Nod> elems;
//    Source::Magnitude::Magnitude* mag;
//    Source::Generator::Type type;
//    Source::Generator::Hardness hardness;
//    std::string filename;
//    std::string label, value;
//    while(!f_in.eof()) {
//        getNextLabelAndValue(label, value);
//        if (label.compare("Type")==0) {
//            type = strToGeneratorType(value);
//        } else if (label.compare("Hardness")==0) {
//            hardness = strToGeneratorHardness(value);
//        } else if (label.compare("Excitation") == 0) {
//            mag = readMagnitude(value);
//        } else if (label.compare("Number of elements")==0) {
//            std::size_t nE = atoi(value.c_str());
//            std::vector<Geometry::ElemId> nodes;
//            nodes.reserve(nE);
//            for (std::size_t i = 0; i < nE; i++) {
//                std::size_t e;
//                f_in >> e;
//                progress_.advance();
//                Geometry::CoordId id = Geometry::CoordId(e);
//                const Geometry::CoordR3* coord = mesh_->coords().getId(id);
//                Geometry::NodR* node =
//                    new Geometry::NodR(Geometry::ElemId(0), &coord);
//                mesh_->elems().addId(node);
//                nodes.push_back(node->getId());
//            }
//            elems = mesh_->elems().getId(nodes);
//        } else if (label.compare("End of Generator")==0) {
//            return new Source::Generator(mag, elems, type, hardness);
//        }
//    }
//    throw std::logic_error("End of Source::Generator label not found.");
//}
//
//Source::OnLine* Parser::readSourceOnLine() {
//    Source::OnLine::Type type;
//    Source::OnLine::Hardness hardness;
//    Source::Magnitude::Magnitude* mag;
//    std::vector<Geometry::ElemId> ids;
//    std::string filename;
//    std::string label, value;
//    while(!f_in.eof()) {
//        getNextLabelAndValue(label, value);
//        if (label.compare("Type")==0) {
//            type = strToNodalType(value);
//        } else if (label.compare("Hardness")==0) {
//            hardness = strToNodalHardness(value);
//        } else if (label.compare("Excitation") == 0) {
//            mag = readMagnitude(value);
//        } else if (label.compare("Number of elements")==0) {
//            std::size_t nE = atoi(value.c_str());
//            ids.reserve(ids.size() + nE);
//            for (size_t i = 0; i < nE; i++) {
//                size_t e;
//                f_in >> e;
//                progress_.advance();
//                ids.push_back(Geometry::ElemId(e));
//            }
//        } else if (label.compare("End of Source_on_line")==0) {
//            Geometry::Element::Group<Geometry::Lin> lines =
//                mesh_->elems().getId(ids);
//            return new Source::OnLine(mag, lines, type, hardness);
//        }
//    }
//    throw std::logic_error("End of Nodal label not found.");
//}
//
//OutputRequest::Base::Type Parser::strToOutputType(std::string str) {
//    str = trim(str);
//    if (str.compare("electricField")==0) {
//        return OutputRequest::Base::electric;
//    } else if (str.compare("magneticField")==0) {
//        return OutputRequest::Base::magnetic;
//    } else if (str.compare("electricFieldNormals")==0) {
//        return OutputRequest::Base::electricFieldNormals;
//    } else if (str.compare("magneticFieldNormals")==0) {
//        return OutputRequest::Base::magneticFieldNormals;
//    } else if (str.compare("current")==0) {
//        return OutputRequest::Base::current;
//    } else if (str.compare("voltage")==0) {
//        return OutputRequest::Base::voltage;
//    } else if (str.compare("bulkCurrentElectric")==0) {
//        return OutputRequest::Base::bulkCurrentElectric;
//    } else if (str.compare("bulkCurrentMagnetic")==0) {
//        return OutputRequest::Base::bulkCurrentMagnetic;
//    } else if (str.compare("farField")==0) {
//        return OutputRequest::Base::electric;
//    } else {
//        throw std::logic_error("Unrecognized output type: " + str);
//    }
//}

Source::Generator::Type Parser::strToGeneratorType(std::string str) {
    str = trim(str);
    if (str.compare("voltage")==0) {
        return Source::Generator::voltage;
    } else if (str.compare("current")==0) {
        return Source::Generator::current;
    } else {
        throw std::logic_error("Unrecognized generator type: " + str);
    }
}

Source::Generator::Hardness Parser::strToGeneratorHardness(std::string str) {
    str = trim(str);
    if (str.compare("soft")==0) {
        return Source::Generator::soft;
    } else if (str.compare("hard")==0) {
        return Source::Generator::hard;
    } else {
        throw std::logic_error("Unrecognized generator hardness: " + str);
    }
}

PhysicalModel::PhysicalModel::Type Parser::strToMaterialType(std::string str) {
    str = trim(str);
    if (str.compare("PEC")==0) {
        return PhysicalModel::PhysicalModel::PEC;
    } else if (str.compare("PMC")==0) {
        return PhysicalModel::PhysicalModel::PMC;
    } else if (str.compare("PML")==0) {
        return PhysicalModel::PhysicalModel::PML;
    } else if (str.compare("SMA")==0) {
        return PhysicalModel::PhysicalModel::SMA;
    } else if (str.compare("Classic")==0) {
        return PhysicalModel::PhysicalModel::classic;
    } else if (str.compare("Dispersive")==0) {
        return PhysicalModel::PhysicalModel::elecDispersive;
    } else if (str.compare("Anisotropic")==0) {
        return PhysicalModel::PhysicalModel::anisotropic;
    } else if (str.compare("SIBC")==0) {
        return PhysicalModel::PhysicalModel::isotropicsibc;
    } else if (str.compare("Wire")==0) {
        return PhysicalModel::PhysicalModel::wire;
    } else if (str.compare("Connector")==0) {
        return PhysicalModel::PhysicalModel::multiport;
    } else if (str.find("Thin_gap")==0) {
        return PhysicalModel::PhysicalModel::gap;
    } else {
        throw std::logic_error("Unrecognized material label: " + str);
    }
}

PhysicalModel::Multiport::Multiport::Type Parser::strToMultiportType(
        std::string str) {
    str = trim(str);
    if (str.compare("Conn_short")==0) {
        return PhysicalModel::Multiport::Multiport::shortCircuit;
    } else if (str.compare("Conn_open")==0) {
        return PhysicalModel::Multiport::Multiport::openCircuit;
    } else if (str.compare("Conn_matched")==0) {
        return PhysicalModel::Multiport::Multiport::matched;
    } else if (str.compare("Conn_sRLC")==0) {
        return PhysicalModel::Multiport::Multiport::sRLC;
    } else if (str.compare("Conn_pRLC")==0) {
        return PhysicalModel::Multiport::Multiport::pRLC;
    } else if (str.compare("Conn_sLpRC")==0) {
        return PhysicalModel::Multiport::Multiport::sLpRC;
    } else if (str.compare("Conn_dispersive") == 0) {
        return PhysicalModel::Multiport::Multiport::dispersive;
    } else {
        throw std::logic_error("Unrecognized multiport label: " + str);
    }
}

std::pair<Math::CVecR3, Math::CVecR3> Parser::strToBox(
        const std::string& value) {
    std::size_t begin = value.find_first_of("{");
    std::size_t end = value.find_last_of("}");
    std::string aux = value.substr(begin+1,end-1);
    std::stringstream iss(aux);
    Math::CVecR3 max, min;
    for (std::size_t i = 0; i < 3; i++) {
        iss >> max(i);
    }
    for (std::size_t i = 0; i < 3; i++) {
        iss >> min(i);
    }
    std::pair<Math::CVecR3,Math::CVecR3> bound(min, max);
    return bound;
}

Math::CVecI3 Parser::strToCVecI3(const std::string& str) {
    std::stringstream ss(str);
    Math::CVecI3 res;
    ss >> res(Math::Constants::x)
       >> res(Math::Constants::y)
       >> res(Math::Constants::z);
    return res;
}

Math::CVecR3 Parser::strToCVecR3(const std::string& str) {
    std::stringstream ss(str);
    Math::CVecR3 res;
    ss >> res(Math::Constants::x)
       >> res(Math::Constants::y)
       >> res(Math::Constants::z);
    return res;
}

Source::OnLine::Type Parser::strToNodalType(std::string str) {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return Source::OnLine::electric;
    } else if (str.compare("magneticField")==0) {
        return Source::OnLine::magnetic;
    } else {
        throw std::logic_error("Unrecognized nodal type: " + str);
    }
}

Source::OnLine::Hardness Parser::strToNodalHardness(std::string str) {
    str = trim(str);
    if (str.compare("soft")==0) {
        return Source::OnLine::soft;
    } else if (str.compare("hard")==0) {
        return Source::OnLine::hard;
    } else {
        throw std::logic_error("Unrecognized nodal hardness: " + str);
        return Source::OnLine::soft;
    }
}

Parser::OutputType Parser::strToGidOutputType(std::string str) {
    str = trim(str);
    if (str.compare("OutRq_on_point")==0) {
        return Parser::outRqOnPoint;
    } else if (str.compare("OutRq_on_line")==0) {
        return Parser::outRqOnLine;
    } else if (str.compare("OutRq_on_surface")==0) {
        return Parser::outRqOnSurface;
    } else if (str.compare("OutRq_on_layer")==0) {
        return Parser::outRqOnLayer;
    } else if (str.compare("Bulk_current_on_surface")==0) {
        return Parser::bulkCurrentOnSurface;
    } else if (str.compare("Bulk_current_on_layer")==0) {
        return Parser::bulkCurrentOnLayer;
    } else if (str.compare("Far_field")==0) {
        return Parser::farField;
    } else {
        throw std::logic_error("Unrecognized label " + str);
        return Parser::outRqOnPoint;
    }
}

Parser::DefinitionMode Parser::strToDefinitionMode(std::string str) {
    str = trim(str);
    if (str.compare("by_vectors")==0) {
        return Parser::byVectors;
    } else if (str.compare("by_angles")==0) {
        return Parser::byAngles;
    } else if (str.compare("randomized_multisource")==0) {
        return Parser::randomizedMultisource;
    } else {
        throw std::logic_error("Unrecognized label " + str);
        return Parser::byVectors;
    }
}

//OutputRequest::Domain Parser::strToDomain(std::string line) {
//    std::size_t timeDomain;
//    Math::Real initialTime;
//    Math::Real finalTime;
//    Math::Real samplingPeriod;
//    std::size_t frequencyDomain;
//    Math::Real initialFrequency;
//    Math::Real finalFrequency;
//    Math::Real frequencyStep;
//    std::size_t logFrequencySweep;
//    std::size_t usingTransferFunction;
//    std::string transferFunctionFile;
//    std::stringstream ss(line);
//    ss >> timeDomain >> initialTime >> finalTime >> samplingPeriod
//       >> frequencyDomain >> initialFrequency >> finalFrequency >>frequencyStep
//       >> logFrequencySweep >> usingTransferFunction >> transferFunctionFile;
//    //
//    transferFunctionFile.erase(std::remove( transferFunctionFile.begin(),
//            transferFunctionFile.end(), '\"' ),transferFunctionFile.end());
//    //
//    return
//        OutputRequest::Domain(
//            toBool(timeDomain), initialTime, finalTime, samplingPeriod,
//            toBool(frequencyDomain), initialFrequency, finalFrequency,
//            frequencyStep,	toBool(logFrequencySweep),
//            toBool(usingTransferFunction), transferFunctionFile);
//}

//Source::Magnitude::Magnitude* Parser::readMagnitude(const std::string typeIn) {
//    std::string type = typeIn;
//    type = trim(type);
//    bool finished = false;
//    std::string label, value;
//    if (type.compare("Gaussian") == 0) {
//        Math::Real delay, spread;
//        bool spreadFound = false;
//        bool delayFound = false;
//        while (!finished && !f_in.eof()) {
//            getNextLabelAndValue(label, value);
//            if (label.compare("Gaussian spread") == 0) {
//                spread = atof(value.c_str());
//                spreadFound = true;
//            } else if (label.compare("Gaussian delay") == 0) {
//                delay = atof(value.c_str());
//                delayFound = true;
//            }
//            finished = spreadFound && delayFound;
//            if (finished) {
//                return new Source::Magnitude::Magnitude(
//                        new Math::Function::Gaussian(spread, delay));
//            }
//        }
//    } else if (type.compare("File") == 0) {
//        std::string excName;
//        while (!finished && !f_in.eof()) {
//            getNextLabelAndValue(label, value);
//            if (label.compare("Filename") == 0) {
//                excName = trim(value);
//                finished = true;
//            }
//            if (finished) {
//                return new Source::Magnitude::Numerical(getFolder() + excName);
//            }
//        }
//    }
//    throw std::logic_error(
//            "Unable to recognize magnitude type when reading excitation.");
//}

bool Parser::checkVersionCompatibility(const std::string& version) {
    bool versionMatches = (version == std::string(OPENSEMBA_VERSION));
    if (!versionMatches) {
        throw std::logic_error(
                "File version " + version + " is not supported.");
    }
    return versionMatches;
}

//PhysicalModel::Volume::PoleResidue Parser::readPoleResiduePair(std::ifstream& stream) {
//    std::string line;
//    std::getline(stream, line);
//    std::size_t prev = 0, pos;
//    std::vector<std::string> wordVector;
//    while ((pos = line.find_first_of(" (,)", prev)) != std::string::npos) {
//        if (pos > prev) {
//            wordVector.push_back(line.substr(prev, pos-prev));
//        }
//        prev = pos+1;
//    }
//    if (prev < line.length()) {
//        wordVector.push_back(line.substr(prev, std::string::npos));
//    }
//    assert(wordVector.size() == 4);
//    PhysicalModel::Volume::PoleResidue res;
//    res.first = std::complex<Math::Real>(
//                    atof(wordVector[0].c_str()),
//                    atof(wordVector[1].c_str()));
//    res.second = std::complex<Math::Real>(
//                    atof(wordVector[2].c_str()),
//                    atof(wordVector[3].c_str()));
//    return res;
//}
//

Math::Axis::Local Parser::strToLocalAxes(const std::string& str) {
    std::size_t begin = str.find_first_of("{");
    std::size_t end = str.find_first_of("}");
    Math::CVecR3 eulerAngles = strToCVecR3(str.substr(begin+1,end-1));
    begin = str.find_last_of("{");
    end = str.find_last_of("}");
    Math::CVecR3 origin = strToCVecR3(str.substr(begin+1,end-1));
    return Math::Axis::Local(eulerAngles, origin);
}

Parser::ParsedElementIds Parser::readElementIds(
        const std::string& str,
        size_t numberOfVertices) {
    Parser::ParsedElementIds res;
    std::stringstream ss(str);

    ss >> res.elemId >> res.mat >> res.layer;
    res.v.resize(8);
    for (std::size_t j = 0; j < numberOfVertices; j++) {
        ss >> res.v[j];
    }

    return res;
}

Parser::ParsedElementPtrs Parser::convertElementIdsToPtrs(
        const ParsedElementIds& elemIds,
        const PhysicalModel::Group<>& physicalModels,
        const Geometry::Layer::Group<>& layers,
        const Geometry::Coordinate::Group<Geometry::CoordR3>& coords) {

    ParsedElementPtrs res;

    if (elemIds.mat != MatId(0)) {
        res.matPtr = physicalModels.getId(elemIds.mat);
    } else {
        res.matPtr = nullptr;
    }

    if (elemIds.layer != Geometry::LayerId(0)) {
        res.layerPtr = layers.getId(elemIds.layer);
    }
    else {
        res.layerPtr = nullptr;
    }

    res.vPtr.resize(elemIds.v.size(), nullptr);
    for (size_t i = 0; i < elemIds.v.size(); ++i) {
        res.vPtr[i] = coords.getId(elemIds.v[i]);
    }

    return res;
}

const PhysicalModel::Bound::Bound* Parser::strToBoundType(std::string str) {
    if (str.compare("PEC") == 0) {
        return new PhysicalModel::Bound::PEC(MatId(0));
    } else if (str.compare("PMC") == 0) {
        return new PhysicalModel::Bound::PMC(MatId(0));
    } else if (str.compare("PML") == 0) {
        return new PhysicalModel::Bound::PML(MatId(0));
    } else if (str.compare("Periodic") == 0) {
        return new PhysicalModel::Bound::Periodic(MatId(0));
    } else if (str.compare("MUR1") == 0) {
        return new PhysicalModel::Bound::Mur1(MatId(0));
    } else if (str.compare("MUR2") == 0) {
        return new PhysicalModel::Bound::Mur2(MatId(0));
    } else {
        throw std::logic_error("Unrecognized bound label: " + str);
    }
}

} /* namespace JSON */
} /* namespace Parser */
} /* namespace SEMBA */
