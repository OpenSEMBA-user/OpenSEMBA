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
namespace GiD {

Parser::Parser() {
    mesh_ = NULL;
    physicalModels_ = NULL;
}

Parser::Parser(const std::string& fn)
:   FileSystem::Project(fn) {
    mesh_ = NULL;
    physicalModels_ = NULL;
}

Parser::~Parser() {

}

Data* Parser::read() {
    if (!canOpen()) {
        throw std::logic_error("Can not openfile: " + getFilename());
    }
    if (isFolder()) {
        throw std::logic_error(getFilename() + " is a directory.");
    }
    openAsInput(f_in);
    std::size_t numLines = 0;
    std::string line;
    while (std::getline(f_in, line)) {
        numLines++;
    }
    f_in.close();
    openAsInput(f_in);
    progress_.init("Parser GiD", numLines, 0);
    std::string version = readVersion();
    if (!checkVersionCompatibility(version)) {
        throw std::logic_error("File version " + version + 
                               " is not supported.");
    }

    Data* res = new Data();
    res->setFilename(getFilename());
    res->solver = readSolver();
    settings_ = res->solver->getSettings();
    pSize_ = readProblemSize();
    physicalModels_ = readMaterials();
    res->physicalModels = physicalModels_;
    mesh_ = readGeometricMesh();
    res->mesh = mesh_;
    res->sources = readEMSources();
    res->outputRequests = readOutputRequests();
    
    postReadOperations(res);

    progress_.end();

    return res;
}

void Parser::printInfo() const {
    std::cout << "--- GiDParser info ---" << std::endl;
    std::cout << "--- End of GiDParser info ---" << std::endl;
}

Solver::Info* Parser::readSolver() {
    Solver::Info* res = NULL;
    bool optionsFound = false;
    while (!optionsFound && !f_in.eof()) {
        std::string label, value;
        getNextLabelAndValue(label, value);
        if (label.compare("Solver options") == 0) {
            optionsFound = true;
            Solver::Settings opts;
            readSolverSettings(opts, "Solver options");
            res = new Solver::Info(value, std::move(opts));
            return res;
        } // Closes problem data found if.
    } // Closes problemDataFound while.
    // Throws error messages if a problem was detected.
    if (res != NULL) {
        delete res;
    }
    throw std::logic_error("No solver options were found.");
}

void Parser::readSolverSettings(Solver::Settings& opts,
                                const std::string& sect) {
    std::string endSect = std::string("End of ") + sect;
    std::string label, value;
    opts.setObject();
    while (!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (trim(label).empty()) {
            continue;
        }
        if (label.find(endSect) != std::string::npos) {
            return;
        } else if (trim(value).empty()) {
            Solver::Settings aux;
            readSolverSettings(aux, label);
            opts.addMember(label, std::move(aux));
        } else {
            Solver::Settings aux;
            aux.setString(value);
            opts.addMember(label, std::move(aux));
        }
    }
}

Geometry::Mesh::Geometric* Parser::readGeometricMesh() {
    const Geometry::Grid3& grid = readCartesianGrid();
    Geometry::Layer::Group<> lG = readLayers();
    Geometry::Coordinate::Group<Geometry::CoordR3> cG = readCoordinates();
    Geometry::Element::Group<Geometry::ElemR> eG = readElements(cG, lG);
    return new Geometry::Mesh::Geometric(grid, cG, eG, lG);
}

Source::Group<>* Parser::readEMSources() {
    Source::Group<>* res = new Source::Group<>();
    bool finished = false;
    bool found = false;
    std::string label, value;
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Excitations")==0) {
            found = true;
            finished = false;
            // Runs over file reading elements excitation defined.
            while (!finished && !f_in.eof() ) {
                getNextLabelAndValue(label,value);
                if (label.compare("Puntual excitation")==0) {
                    res->add(readDipole());
                } else if (label.compare("Planewave")==0) {
                    res->add(readPlaneWave());
                } else if (label.compare("Generator")==0) {
                    res->add(readGenerator());
                } else if (label.compare("Waveguide_port")==0) {
                    res->add(readPortWaveguide());
                } else if (label.compare("TEM_port")==0) {
                    res->add(readPortTEM());
                } else if (label.compare("Source_on_line")==0) {
                    res->add(readSourceOnLine());
                } else if (label.compare("End of Excitations")==0) {
                    finished = true;
                } // if: end of boundary conditions label was found.
            } // while: loops until eof or the task is finished.
        } // if: boundary condition label was found.
    } // while: information was found or eof was reached.
    if (!found) {
        throw std::logic_error("Excitations label was not found.");
    }
    //
    return res;
}

PhysicalModel::Group<>* Parser::readMaterials(){
    PhysicalModel::Group<>* res = new PhysicalModel::Group<>();
    std::string label, value;
    std::size_t materialCount = 0;
    while (!f_in.eof() && label.compare("End of materials")!=0) {
        getNextLabelAndValue(label, value);
        if (label.compare("Material")==0) {
            materialCount++;
            MatId id = MatId(atoi(value.c_str()));
            res->add(readPhysicalModel(id));
        }
    }
    assert(materialCount == pSize_.mat);
    return res;
}

PhysicalModel::PhysicalModel* Parser::readPhysicalModel(const MatId id) {
    std::string name;
    PhysicalModel::PhysicalModel::Type type;
    PhysicalModel::Multiport::Multiport::Type mpType =
        PhysicalModel::Multiport::Multiport::undefined;
    SIBCType surfType = undefinedSIBC;
    std::string layersStr;
    Math::Real rEps, rMu, eC, mC;
    PhysicalModel::Volume::Anisotropic::Model anisotropicModel;
    Math::Real crystalRMu, ferriteREps, ferriteRMu;
    Math::CVecR3 rEpsPrincipalAxes;
    Math::Axis::Local localAxes;
    bool pmlAutomaticOrientation;
    Math::Real kappa;
    Math::Real radius, R, L, C, P_R, P_L, P_C;
    Math::Real width;
    bool wireDispersive = false;
    bool wireSeriesParallel = false;
    FileSystem::Project file;
    while (!f_in.eof()) {
        std::string label, value;
        getNextLabelAndValue(label, value);
        if (label.compare("Name")==0) {
            name = trim(value);
        } else if (label.compare("TypeId")==0) {
            type = strToMaterialType(value);
            if (type == PhysicalModel::PhysicalModel::multiport) {
                mpType = strToMultiportType(value);
            }
        } else if (label.compare("Permittivity")==0) {
            rEps = atof(value.c_str());
        } else if (label.compare("Permeability")==0) {
            rMu = atof(value.c_str());
        } else if (label.compare("Electric Conductivity")==0) {
            eC = atof(value.c_str());
        } else if (label.compare("Magnetic Conductivity")==0) {
            mC = atof(value.c_str());
        } else if (label.compare("Automatic Orientation")==0) {
            pmlAutomaticOrientation = strToBool(value);
        } else if (label.compare("WireType") == 0) {
            if (value == "SeriesParallel") {
                wireSeriesParallel = true;
            } else if (value == "Dispersive") {
                wireDispersive = true;
            }
        } else if (label.compare("Radius")==0) {
            radius = atof(value.c_str());
        } else if (label.compare("Resistance")==0) {
            R = atof(value.c_str());
        } else if (label.compare("Inductance")==0) {
            L = atof(value.c_str());
        } else if (label.compare("Capacitance")==0) {
            C = atof(value.c_str());
        } else if (label.compare("Parallel Resistance") == 0) {
            P_R = atof(value.c_str());
        } else if (label.compare("Parallel Inductance") == 0) {
            P_L = atof(value.c_str());
        } else if (label.compare("Parallel Capacitance") == 0) {
            P_C = atof(value.c_str());
        } else if (label.compare("Width") == 0) {
            width = atof(value.c_str());
        } else if (label.compare("SurfaceType")==0) {
            surfType = strToSIBCType(value);
        } else if (label.compare("Filename")==0) {
            file = FileSystem::Project(getFolder() + trim(value));
        } else if (label.compare("Layers")==0) {
            layersStr = value;
        } else if (label.compare("Anisotropic model") == 0) {
            anisotropicModel = strToAnisotropicModel(value);
        } else if (label.compare("Local Axes") == 0) {
            localAxes = strToLocalAxes(value);
        } else if (label.compare("Relative permittivity principal axes") == 0) {
            rEpsPrincipalAxes = strToCVecR3(value);
        } else if (label.compare("Crystal relative permeability") == 0) {
            crystalRMu = atof(value.c_str());
        } else if (label.compare("Kappa") == 0) {
            kappa = atof(value.c_str());
        } else if (label.compare("Ferrite relative permeability") == 0) {
            ferriteRMu = atof(value.c_str());
        } else if (label.compare("Ferrite relative permittivity") == 0) {
            ferriteREps = atof(value.c_str());
        } else if (label.compare("End of Material")==0) {
            // Creates material.
            switch (type) {
            case PhysicalModel::PhysicalModel::PEC:
                return new PhysicalModel::Predefined::PEC(id, name);
            case PhysicalModel::PhysicalModel::PMC:
                return new PhysicalModel::Predefined::PMC(id, name);
            case PhysicalModel::PhysicalModel::SMA:
                return new PhysicalModel::Predefined::SMA(id, name);
            case PhysicalModel::PhysicalModel::PML:
                if (pmlAutomaticOrientation) {
                    return new PhysicalModel::Volume::PML(id, name, NULL);
                } else {
                    return new PhysicalModel::Volume::PML(
                            id, name, new Math::Axis::Local(localAxes));
                }
            case PhysicalModel::PhysicalModel::classic:
                return new PhysicalModel::Volume::Classic(
                        id, name, rEps, rMu, eC, mC);
            case PhysicalModel::PhysicalModel::elecDispersive:
                return new PhysicalModel::Volume::Dispersive(id, name, file);
            case PhysicalModel::PhysicalModel::anisotropic:
                switch (anisotropicModel) {
                case PhysicalModel::Volume::Anisotropic::Model::crystal:
                    return new PhysicalModel::Volume::AnisotropicCrystal(
                            id, name, localAxes,
                            rEpsPrincipalAxes, crystalRMu);
                case PhysicalModel::Volume::Anisotropic::Model::ferrite:
                    return new PhysicalModel::Volume::AnisotropicFerrite(
                            id, name, localAxes,
                            kappa,ferriteRMu,ferriteREps);
                default:
                    throw std::logic_error("Material type not recognized.");
                }
            case PhysicalModel::PhysicalModel::isotropicsibc:
                switch (surfType) {
                case sibc:
                    return new PhysicalModel::Surface::SIBC(id, name, file);
                case multilayer:
                    return readMultilayerSurf(id, name, layersStr);
                default:
                    throw std::logic_error("Undefined SIBC Type.");
                }
                break;
            case PhysicalModel::PhysicalModel::wire:
                if (wireSeriesParallel) {
                    return new PhysicalModel::Wire::Wire(id, name, radius,
                                                         R, L, C,
                                                         P_R, P_L, P_C);
                }
                if (wireDispersive) {
                    return new PhysicalModel::Wire::Wire(id, name,
                                                         radius, file);
                }
                return new PhysicalModel::Wire::Wire(id, name, radius, R, L);
            case PhysicalModel::PhysicalModel::gap:
                return new PhysicalModel::Gap::Gap(id, name, width);
            case PhysicalModel::PhysicalModel::multiport:
                if (mpType ==
                        PhysicalModel::Multiport::Multiport::shortCircuit) {
                    return new PhysicalModel::Multiport::Predefined(
                            id, name, mpType);
                } else if (mpType ==
                            PhysicalModel::Multiport::Multiport::dispersive) {
                    return new PhysicalModel::Multiport::Dispersive(
                            id, name, file);
                } else {
                    return new PhysicalModel::Multiport::RLC(
                            id, name, mpType, R, L, C);
                }
            default:
                throw std::logic_error("Material type not recognized.");
            }
        }
    }
    return NULL;
}

OutputRequest::Group<>* Parser::readOutputRequests() {
    OutputRequest::Group<>* res = new OutputRequest::Group<>();
    bool finished;
    bool found = false;
    std::string line, label, value;
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Output Requests")==0) {
            found = true;
            finished = false;
            while (!finished && !f_in.eof() ) {
                getNextLabelAndValue(label, value);
                if (label.compare("Output request instance")==0) {
                    readOutRqInstances(res);
                } else if (label.compare("End of Output Requests")==0) {
                    finished = true;
                }
            }
        }
    }
    return res;
}

Geometry::Element::Group<> Parser::boxToElemGroup(const std::string& line) {
    Geometry::BoxR3 box = strToBox(line);
    if (box.isVolume()) {
        Geometry::HexR8* hex = 
            new Geometry::HexR8(*mesh_, Geometry::ElemId(0), box);
        mesh_->elems().addId(hex);
        Geometry::Element::Group<> elems(hex);
        return elems;
    } else if (box.isSurface()) {
        Geometry::QuaR4* qua = 
            new Geometry::QuaR4(*mesh_, Geometry::ElemId(0), box);
        mesh_->elems().addId(qua);
        Geometry::Element::Group<> elems(qua);
        return elems;
    } else {
        throw std::logic_error("Box to Elem Group only works "
                               "for volumes and surfaces");
    }
}

void Parser::readOutRqInstances(OutputRequest::Group<>* res) {
    bool finished = false;
    GiDOutputType gidOutputType;
    while (!finished && !f_in.eof()) {
        std::string line, label, value;
        getNextLabelAndValue(label,value);
        if (label.compare("GiDOutputType")==0) {
            gidOutputType = strToGidOutputType(trim(value));
        } else if (label.compare("Number of elements")==0) {
            std::size_t nE = atoi(value.c_str());
            for (std::size_t i = 0; i < nE; i++) {
                getNextLabelAndValue(label,value);
                std::string name = trim(value);
                getNextLabelAndValue(label,value);
                OutputRequest::Base::Type type = strToOutputType(trim(value));
                getNextLabelAndValue(label,value);
                OutputRequest::Domain domain = strToDomain(value);
                switch (gidOutputType) {
                case Parser::outRqOnPoint:
                {
                    getNextLabelAndValue(label,value);
                    Geometry::CoordId coordId(atoi(value.c_str()));
                    const Geometry::CoordR3* coord =
                        mesh_->coords().getId(coordId);
                    Geometry::NodR* node = 
                        new Geometry::NodR(Geometry::ElemId(0), &coord);
                    mesh_->elems().addId(node);
                    Geometry::Element::Group<Geometry::Nod> elems(node);
                    res->add(
                        new OutputRequest::OutputRequest<Geometry::Nod>(
                            domain, type, name, elems));
                    break;
                }
                //case Parser::outRqOnLine:
                //    getNextLabelAndValue(label,value);
                //    elem.clear();
                //    elem.push_back(Geometry::ElemId(atoi(value.c_str())));
                //    res->add(new OutRq(elem, domain, type, name));
                //    break;
                case Parser::outRqOnSurface:
                {
                    getNextLabelAndValue(label,value);
                    std::vector<Geometry::ElemId> ids;
                    ids.push_back(Geometry::ElemId(atoi(value.c_str())));
                    Geometry::Element::Group<Geometry::ElemR> elems = 
                        mesh_->elems().getId(ids);
                    Geometry::Element::Group<Geometry::Surf> surfs = 
                        elems.getOf<Geometry::Surf>();
                    res->add(
                        new OutputRequest::OutputRequest<Geometry::Surf>(
                            domain, type, name, surfs));
                    break;
                }
                case Parser::outRqOnVolume:
                {
                    getline_(line);
                    Geometry::Element::Group<> elems = boxToElemGroup(line);
                    if (elems.getOf<Geometry::Vol>().size()) {
                        res->add(
                            new OutputRequest::OutputRequest<Geometry::Vol>(
                                domain, type, name,
                                elems.getOf<Geometry::Vol>()));
                    } else if (elems.getOf<Geometry::Surf>().size()) {
                        res->add(
                            new OutputRequest::OutputRequest<Geometry::Surf>(
                                domain, type, name,
                                elems.getOf<Geometry::Surf>()));
                    } else {
                        throw std::logic_error("Layer for OutRq on volume must"
                                               " be volume or surface");
                    }
                    break;
                }
                case Parser::bulkCurrentOnSurface:
                {
                    Math::Constants::CartesianAxis dir;
                    std::size_t skip;
                    getNextLabelAndValue(label,value);
                    switch (value[0]) {
                    case 'x':
                        dir = Math::Constants::x;
                        break;
                    case 'y':
                        dir = Math::Constants::y;
                        break;
                    case 'z':
                        dir = Math::Constants::z;
                        break;
                    default:
                        dir = Math::Constants::x;
                    }
                    getNextLabelAndValue(label,value);
                    skip = atoi(value.c_str());
                    getline_(line);
                    std::vector<Geometry::ElemId> ids;
                    ids.push_back(Geometry::ElemId(atoi(line.c_str())));
                    Geometry::Element::Group<Geometry::ElemR> elems = 
                        mesh_->elems().getId(ids);
                    Geometry::Element::Group<Geometry::Surf> surfs =
                        elems.getOf<Geometry::Surf>();
                    res->add(
                        new OutputRequest::BulkCurrent(
                            domain, name, surfs, dir, skip));
                    break;
                }
                case Parser::bulkCurrentOnVolume:
                {
                    Math::Constants::CartesianAxis dir;
                    std::size_t skip;
                    getNextLabelAndValue(label,value);
                    switch (value[0]) {
                    case 'x':
                        dir = Math::Constants::x;
                        break;
                    case 'y':
                        dir = Math::Constants::y;
                        break;
                    case 'z':
                        dir = Math::Constants::z;
                        break;
                    default:
                        dir = Math::Constants::x;
                    }
                    getNextLabelAndValue(label,value);
                    skip = atoi(value.c_str());
                    getline_(line);
                    Geometry::Element::Group<Geometry::Vol> elems = 
                        boxToElemGroup(line);
                    res->add(
                        new OutputRequest::BulkCurrent(
                            domain, name, elems, dir, skip));
                    break;
                }
                case Parser::farField:
                {
                    getline_(line);
                    Geometry::Element::Group<Geometry::Vol> elems =
                        boxToElemGroup(line);
                    Math::Real iThDeg, fThDeg, sThDeg, iPhiDeg, fPhiDeg, sPhiDeg;
                    f_in >> iThDeg >> fThDeg >> sThDeg
                        >> iPhiDeg >> fPhiDeg >> sPhiDeg;
                    getline_(line);
                    Math::Real iThRad, fThRad, sThRad, iPhiRad, fPhiRad, sPhiRad;
                    static const Math::Real degToRad = 2.0 * Math::Constants::pi / 360.0;
                    iThRad = iThDeg * degToRad;
                    fThRad = fThDeg * degToRad;
                    sThRad = sThDeg * degToRad;
                    iPhiRad = iPhiDeg * degToRad;
                    fPhiRad = fPhiDeg * degToRad;
                    sPhiRad = sPhiDeg * degToRad;
                    OutputRequest::FarField* oRFF =
                        new OutputRequest::FarField(
                            domain, name, elems,
                            iThRad, fThRad, sThRad, iPhiRad, fPhiRad, sPhiRad);
                    res->add(oRFF);
                    break;
                }
                default:
                    throw std::logic_error(
                            "Unrecognized GiD Output request type:" + type);
                }
            } // End of loop running over the elements.
        } else if (label.compare("End of Output request instance")==0) {
            finished = true;
        } else {
            throw std::logic_error("Label not identified: " + label);
        } // End of condition comparing labels.
    }
}

ProblemSize Parser::readProblemSize() {
    std::string label, line, value;
    bool finished = false;
    bool problemSizeFound = false;
    ProblemSize res;
    // Runs until "Problem Size" is found or eof is reached.
    while (!problemSizeFound && !f_in.eof()) {
        getline_(line);
        if (line.find("Problem size:") != line.npos ) {
            problemSizeFound = true;
            while(!finished) {
                getNextLabelAndValue(label, value);
                if (label.compare("Hexahedral elements")==0) {
                    res.hex8 = atoi(value.c_str());
                } else if (label.compare("Quadratic Tetrahedral elements")==0) {
                    res.tet10 = atoi(value.c_str());
                } else if (
                        label.compare("Linear Tetrahedral elements")==0 ||
                        label.compare("Tetrahedral elements")==0) {
                    res.tet4 = atoi(value.c_str());
                } else if (label.compare("Quadratic Triangular elements")==0) {
                    res.tri6 = atoi(value.c_str());
                } else if (label.compare("Linear Triangular elements")==0
                        || label.compare("Triangular elements")==0) {
                    res.tri3 = atoi(value.c_str());
                } else if (label.compare("Quadratic Line elements")==0) {
                    res.lin3 = atoi(value.c_str());
                } else if (
                        label.compare("Linear Line elements")==0 ||
                        label.compare("Line elements")==0) {
                    res.lin2 = atoi(value.c_str());
                } else if (label.compare("Number of points")==0) {
                    res.v = atoi(value.c_str());
                } else if (label.compare("Number of materials")==0) {
                    res.mat = atoi(value.c_str());
                } else if(label.find("End of problem size") != label.npos) {
                    finished = true;
                }
                if (f_in.eof()) {
                    throw std::logic_error(
                            "End of problem size label not found");
                }
            }
        }
    }
    if (!problemSizeFound) {
        throw std::logic_error("End of problem size label not found");
    }
    return res;
}

Geometry::Layer::Group<> Parser::readLayers() {
    bool finished = false;
    bool found = false;
    std::string label, value;
    Geometry::Layer::Id id;
    Geometry::Layer::Group<> res;
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Layers")==0) {
            found = true;
            while(!finished && !f_in.eof()) {
                std::string line;
                getline(f_in, line);
                if (line.find("End of layers") != line.npos) {
                    finished = true;
                } else {
                    std::stringstream ss(line);
                    ss >> id;;
                    std::string name;
                    name = line.substr(line.find_first_of(" ") + 1,
                                       line.length());
                    res.add(new Geometry::Layer::Layer(id, name));
                }
            }
        }
    }
    if (!found) {
        throw std::logic_error("Layers label was not found.");
    }
    return res;
}

Geometry::Coordinate::Group<Geometry::CoordR3> Parser::readCoordinates() {
    std::string line;
    Geometry::CoordId id;
    Math::CVecR3 pos;
    std::vector<Geometry::CoordR3*> coord;
    coord.reserve(pSize_.v);
    bool finished = false;
    bool found = false;
    while (!found && !f_in.eof() && !finished) {
        getline_(line);
        if (line.find("Coordinates:") != line.npos) {
            found = true;
            // Reads coordinates.
            for (std::size_t i = 0; i < pSize_.v; i++) {
                f_in >> id >> pos(0) >> pos(1) >> pos(2);
                progress_.advance();
                coord.push_back(new Geometry::CoordR3(id, pos));
            }
            // Checks "end of coordinates" label.
            finished = false;
            while (!finished && !f_in.eof()) {
                getline_(line);
                if (line.find("End of coordinates") != line.npos) {
                    finished = true;
                }
            }
        }
    }
    if (!found) {
        throw std::logic_error("Coordinates label was not found.");
    }
    if (!finished) {
        throw std::logic_error("End of coordinates label not found.");
    }
    return Geometry::Coordinate::Group<Geometry::CoordR3>(coord);
}

Geometry::Element::Group<Geometry::ElemR> Parser::readElements(
        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
        const Geometry::Layer::Group<>& lG) {
    std::string line, label;
    bool finished = false;
    bool found = false;
    std::vector<Geometry::ElemR*> elems;
    while (!finished && !f_in.eof()) {
        getline_(line);
        if (line.find("Elements:") != line.npos) {
            found = true;
        }
        // Stores data label in labelStr std::string.
        label = line.substr(0, line.find(LABEL_ENDING));
        if (label.compare("Linear Hexahedral Elements")==0 ||
                label.compare("Hexahedral Elements")==0) {
            readHex8Elements(cG, lG, elems);
        } else if (label.compare("Quadratic Tetrahedral Elements")==0) {
            readTet10Elements(cG, lG, elems);
        } else if (label.compare("Linear Tetrahedral Elements")==0 ||
                   label.compare("Tetrahedral Elements")==0) {
            readTet4Elements(cG, lG, elems);
        } else if (label.compare("Quadratic Triangle Elements")==0) {
            readTri6Elements(cG, lG, elems);
        } else if (label.compare("Linear Triangle Elements")==0 ||
                   label.compare("Triangle Elements")==0) {
            readTri3Elements(cG, lG, elems);
        } else if (label.compare("Linear Line Elements")==0 ||
                   label.compare("Line Elements")==0) {
            readLin2Elements(cG, lG, elems);
        } else if(label.find("End of Elements") != label.npos) {
            finished = true;
        }
    }
    // Shows error message if the elements label was not found.
    if (!found) {
        throw std::logic_error("\"Elements\" label was not found.");
    }
    // This code is reached only in case of "End of elements" is not found.
    if (!finished) {
        throw std::logic_error("\"End of elements\" label was not found.");
    }
    //
    return Geometry::Element::Group<Geometry::ElemR>(elems);
}

void Parser::readHex8Elements(
        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
        const Geometry::Layer::Group<>& lG,
        std::vector<Geometry::ElemR*>& elems) {
    Geometry::ElemId id;
    Geometry::CoordId vId;
    const Geometry::CoordR3* v[8];
    MatId matId;
    for (std::size_t i = 0; i < pSize_.hex8; i++) {
        f_in >> id;
        for (std::size_t j = 0; j < 8; j++) {
            f_in >> vId;
            v[j] = cG.getId(vId);
        }
        f_in >> matId;
        progress_.advance();
        const PhysicalModel::PhysicalModel* mat;
        if (matId != MatId(0)) {
            mat = physicalModels_->getId(matId);
        } else {
            mat = NULL;
        }
        elems.push_back(new Geometry::HexR8(id, v, NULL, mat));
    }
}

void Parser::readTet10Elements(
        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
        const Geometry::Layer::Group<>& lG,
        std::vector<Geometry::ElemR*>& elems) {
    Geometry::ElemId id;
    Geometry::CoordId vId;
    const Geometry::CoordR3* v[10];
    MatId matId;
    for (std::size_t i = 0; i < pSize_.tet10; i++) {
        f_in >> id;
        for (std::size_t j = 0; j < 10; j++) {
            f_in >> vId;
            v[j] = cG.getId(vId);
        }
        f_in >> matId;
        progress_.advance();
        const PhysicalModel::PhysicalModel* mat;
        if (matId != MatId(0)) {
            mat = physicalModels_->getId(matId);
        }
        else {
            mat = NULL;
        }
        elems.push_back(new Geometry::Tet10(id, v, NULL, mat));
    }
}

void Parser::readTet4Elements(
        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
        const Geometry::Layer::Group<>& lG,
        std::vector<Geometry::ElemR*>& elems) {
    Geometry::ElemId id;
    Geometry::CoordId vId;
    const Geometry::CoordR3* v[4];
    Geometry::Layer::Id layerId;
    MatId matId;
    for (std::size_t i = 0; i < pSize_.tet4; i++) {
        f_in >> id;
        for (std::size_t j = 0; j < 4; j++) {
            f_in >> vId;
            v[j] = cG.getId(vId);
        }
        f_in >> matId >> layerId;
        progress_.advance();
        const Geometry::Layer::Layer* lay;
        if (layerId != Geometry::LayerId(0)) {
            lay = lG.getId(layerId);
        }
        else {
            lay = NULL;
        }
        const PhysicalModel::PhysicalModel* mat;
        if (matId != MatId(0)) {
            mat = physicalModels_->getId(matId);
        }
        else {
            mat = NULL;
        }
        elems.push_back(new Geometry::Tet4(id, v, lay, mat));
    }
}

void Parser::readTri6Elements(
        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
        const Geometry::Layer::Group<>& lG,
        std::vector<Geometry::ElemR*>& elems) {
    Geometry::ElemId id;
    Geometry::CoordId vId;
    const Geometry::CoordR3* v[6];
    MatId matId;
    Math::CVecR3 normal;
    for (std::size_t i = 0; i < pSize_.tri6; i++) {
        f_in >> id;
        for (std::size_t j = 0; j < 6; j++) {
            f_in >> vId;
            v[j] = cG.getId(vId);
        }
        f_in >> matId;
        progress_.advance();
        const PhysicalModel::PhysicalModel* mat;
        if (matId != MatId(0)) {
            mat = physicalModels_->getId(matId);
        }
        else {
            mat = NULL;
        }
        elems.push_back(new Geometry::Tri6(id, v, NULL, mat));
    }
}

void Parser::readTri3Elements(
        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
        const Geometry::Layer::Group<>& lG,
        std::vector<Geometry::ElemR*>& elems) {
    Geometry::ElemId id;
    Geometry::CoordId vId;
    const Geometry::CoordR3* v[3];
    Geometry::Layer::Id layerId;
    MatId matId;
    Math::CVecR3 normal;
    for (std::size_t i = 0; i < pSize_.tri3; i++) {
        f_in >> id;
        for (std::size_t j = 0; j < 3; j++) {
            f_in >> vId;
            v[j] = cG.getId(vId);
        }
        f_in >> matId >> layerId;
        progress_.advance();
        const Geometry::Layer::Layer* lay;
        if (layerId != Geometry::LayerId(0)) {
            lay = lG.getId(layerId);
        }
        else {
            lay = NULL;
        }
        const PhysicalModel::PhysicalModel* mat;
        if (matId != MatId(0)) {
            mat = physicalModels_->getId(matId);
        }
        else {
            mat = NULL;
        }
        elems.push_back(new Geometry::Tri3(id, v, lay, mat));
    }
}

void Parser::readLin2Elements(
        const Geometry::Coordinate::Group<Geometry::CoordR3>& cG,
        const Geometry::Layer::Group<>& lG,
        std::vector<Geometry::ElemR*>& elems) {
    Geometry::ElemId id;
    Geometry::CoordId vId;
    const Geometry::CoordR3* v[2];
    Geometry::Layer::Id layerId;
    MatId matId;
    for (std::size_t i = 0; i < pSize_.lin2; i++) {
        f_in >> id;
        for (std::size_t j = 0; j < 2; j++) {
            f_in >> vId;
            v[j] = cG.getId(vId);
        }
        f_in >> matId >> layerId;
        progress_.advance();
        const Geometry::Layer::Layer* lay;
        if (layerId != Geometry::LayerId(0)) {
            lay = lG.getId(layerId);
        }
        else {
            lay = NULL;
        }
        const PhysicalModel::PhysicalModel* mat;
        if (matId != MatId(0)) {
            mat = physicalModels_->getId(matId);
        }
        else {
            mat = NULL;
        }
        elems.push_back(new Geometry::LinR2(id, v, lay, mat));
    }
}

PhysicalModel::Volume::Dispersive* Parser::readDispersiveMatFile(
        const MatId id,
        const std::string& name,
        const FileSystem::Project& file) const {
    if (!file.canOpen()) {
        throw std::ios_base::failure(file);
    }
    std::ifstream stream;
    file.openAsInput(stream);
    Math::Real sig, eps, mu, sigM;
    stream >> sig >> eps >> mu >> sigM;
    std::size_t nPoles, trash;
    stream >> nPoles >> trash >> trash >> trash;
    std::vector<PhysicalModel::Volume::PoleResidue> poleResidues;
    for (std::size_t i = 0; i < nPoles; i++) {
        PhysicalModel::Volume::PoleResidue pR = readPoleResiduePair(stream);
        poleResidues.push_back(pR);
    }
    return new PhysicalModel::Volume::Dispersive(id, name,
            eps / Math::Constants::eps0,
            mu / Math::Constants::mu0,
            sig, sigM, poleResidues);
}

PhysicalModel::Surface::Multilayer* Parser::readMultilayerSurf(
        const MatId id,
        const std::string& name,
        const std::string& layersStr) const {
    std::size_t begin = layersStr.find_first_of("\"");
    std::size_t end = layersStr.find_last_of("\"");
    std::istringstream ss(layersStr.substr(begin+1,end-2));
    std::string sub;
    std::vector<Math::Real> thick, rEps, rMu, eCond, mCond;
    std::size_t parameters;
    std::string trash;
    ss >> trash >> parameters;
    const std::size_t nLayers = parameters / 5;
    for (std::size_t i = 0; i < nLayers; i++) {
        // Thickness, Permittivity, Permeability, ElecCond, MagnCond.
        Math::Real thick_, rEps_, rMu_, eCond_, mCond_;
        ss >> thick_ >> rEps_ >> rMu_ >> eCond_ >> mCond_;
        thick.push_back(thick_);
        rEps.push_back(rEps_);
        rMu.push_back(rMu_);
        eCond.push_back(eCond_);
        mCond.push_back(mCond_);
    }
    return new PhysicalModel::Surface::Multilayer(
            id, name, thick, rEps, rMu, eCond, mCond);
}

PhysicalModel::Surface::SIBC* Parser::readIsotropicSurfMatFile(
        const MatId id,
        const std::string& fileName,
        const FileSystem::Project& file) const {
    std::ifstream matFile;
    std::string line, label, value;
    std::string name, model;
    char *pEnd;
    Math::Matrix::Static<Math::Real,2,2> Zstatic, Zinfinite;
    std::vector<Math::Real> pole;
    std::vector<Math::Matrix::Static<Math::Real,2,2> > Z;
    Math::Real tmpP;
    // Opens file, read only mode.
    matFile.open(fileName.c_str(), std::ifstream::in);
    if (matFile.fail()) {
        throw std::logic_error("Problem opening file: " + fileName);
    }
    // Parses first line, containing material name.
    getline(matFile, line);
    if (line.find("#PANEL#") == std::string::npos) {
        throw std::logic_error("#PANEL# label has not been found in first line");
    }
    name = line.substr(8, line.length()-9);
    getline(matFile, line);
    std::size_t nPoles = 0;
    // Gets number of poles.
    label = line.substr(0, line.find(":"));
    if(!label.compare("N")) {
        value  = line.substr(line.find(":")+2, line.length());
        nPoles = atoi(value.c_str());
    }
    // Gets Zinfinite.
    getline(matFile, line);
    label = line.substr(0, line.find(":"));
    if(!label.compare("Zinf")) {
        value  = line.substr(line.find(":")+2, line.length());
        Zinfinite(0,0) = strtod(value.c_str(), &pEnd);
        Zinfinite(0,1) = strtod(pEnd, &pEnd);
        Zinfinite(1,0) = strtod(pEnd, &pEnd);
        Zinfinite(1,1) = strtod(pEnd, &pEnd);
    }
    // Gets Zstatic.
    getline(matFile, line);
    label = line.substr(0, line.find(":"));
    if(!label.compare("Zstatic")) {
        value  = line.substr(line.find(":")+2, line.length());
        Zstatic(0,0) = strtod(value.c_str(), &pEnd);
        Zstatic(0,1) = strtod(pEnd, &pEnd);
        Zstatic(1,0) = strtod(pEnd, &pEnd);
        Zstatic(1,1) = strtod(pEnd, &pEnd);
    }
    // Parses poles.
    // Stores in line the file line containing headers.
    getline(matFile, line);
    for (std::size_t i = 0; i < nPoles; i++) {
        Math::MatR22 tmpZ;
        matFile >> tmpP >> tmpZ(0,0) >> tmpZ(0,1) >> tmpZ(1,0) >> tmpZ(1,1);
        pole.push_back(tmpP);
        Z.push_back(tmpZ);
    }
    // Copies all parsed data into the aux material depending on the model.
    return new PhysicalModel::Surface::SIBC(
            id, name, Zinfinite, Zstatic, pole, Z);
}

void Parser::getNextLabelAndValue(std::string& label, std::string& value) {
    std::string line;
    getline_(line);
    line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    label = line.substr(0, line.find(LABEL_ENDING));
    label = trim(label);
    value = line.substr(line.find(LABEL_ENDING) + 1, line.length());
    value = trim(value);
}

Geometry::Grid3 Parser::readCartesianGrid() {
    std::string label, line, value;
    bool paddingByNumberOfCells;
    bool finished = false;
    bool gridLabelFound = false;
    bool gridFound = false;
    Geometry::Grid3 grid;
    Geometry::BoxR3 bound;
    std::pair<Math::CVecR3, Math::CVecR3> boundaryPadding, boundaryMeshSize;
    bool stepsByNumberOfCells = true;
    Math::CVecI3 numElems;
    Math::CVecR3 steps;
    while (!gridLabelFound && !f_in.eof()) {
        getline_(line);
        if (line.find("Grid:") != line.npos ) {
            gridLabelFound = true;
            while(!finished) {
                getNextLabelAndValue(label, value);
                if (label.compare("Layer Box")==0) {
                    gridFound = true;
                    bound = Geometry::BoxR3(strToBox(value));
                } else if (label.compare("Type")==0) {
                    if (trim(value).compare("by_number_of_cells")==0) {
                        stepsByNumberOfCells = true;
                    } else {
                        stepsByNumberOfCells = false;
                    }
                } else if (label.compare("Directions")==0) {
                    Math::CVecR3 aux = strToCartesianVector(value);
                    if (stepsByNumberOfCells) {
                        numElems(0) = (Math::Int) aux(0);
                        numElems(1) = (Math::Int) aux(1);
                        numElems(2) = (Math::Int) aux(2);
                        if (numElems == Math::CVecR3()) {
                            std::cerr << "WARNING @ Parser: "
                                 << "Number of grid cells is zero. "
                                 << "Definition by number of cells was used. "
                                 << std::endl;
                        }
                    } else {
                        steps = aux;
                    }
                } else if (label.compare("Boundary padding type") == 0) {
                    if (trim(value).compare("by_number_of_cells")==0) {
                        paddingByNumberOfCells = true;
                    } else {
                        paddingByNumberOfCells = false;
                    }
                } else if (label.compare("Boundary padding") == 0) {
                    boundaryPadding = strToBox(value);
                } else if (label.compare("Boundary mesh size") == 0) {
                    boundaryMeshSize = strToBox(value);
                } else if(label.find("End of Grid") != label.npos) {
                    finished = true;
                    if (!gridFound) {
                        throw std::logic_error("Grid not found");
                    }
                }
                if (f_in.eof()) {
                    throw std::logic_error("End of grid label not found");
                }
            }
        }
    }
    if (paddingByNumberOfCells) {
        boundaryPadding.first =
                boundaryPadding.first  * boundaryMeshSize.first;
        boundaryPadding.second =
                boundaryPadding.second * boundaryMeshSize.second;
    }
    // Throws error message if label was not found.
    if (!gridLabelFound) {
        throw std::logic_error("Grid3 label not found.");
    }
    if (gridFound) {
        if (stepsByNumberOfCells) {
            grid = Geometry::Grid3(bound, numElems);
        } else {
            grid = Geometry::Grid3(bound, steps);
        }
        grid.enlarge(boundaryPadding, boundaryMeshSize);
    }
    return grid;
}

Source::PlaneWave* Parser::readPlaneWave() {
    std::string filename;
    std::string label, value;
    DefinitionMode definitionMode;
    Math::CVecR3 dir, pol;
    Math::Real theta, phi, alpha, beta;
    static const Math::Real degToRad = 2.0 * Math::Constants::pi / 360.0;
    Geometry::Element::Group<Geometry::Vol> elems;
    Source::Magnitude::Magnitude* mag;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Definition mode")==0) {
            definitionMode = strToDefinitionMode(value);
        } else if (label.compare("Direction vector")==0) {
            dir = strToCartesianVector(value);
        } else if (label.compare("Polarization vector") == 0) {
            pol = strToCartesianVector(value);
        } else if (label.compare("Direction theta")==0) {
            theta = atof(value.c_str()) * degToRad;
        } else if (label.compare("Direction phi")==0) {
            phi = atof(value.c_str()) * degToRad;
        } else if (label.compare("Polarization alpha")==0) {
            alpha = atof(value.c_str()) * degToRad;
        } else if (label.compare("Polarization beta")==0) {
            beta = atof(value.c_str()) * degToRad;
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Layer Box") == 0) {
            elems = boxToElemGroup(value);
        } else if (label.compare("Number of elements")==0) {
            std::size_t nE = atoi(value.c_str());
            elems.clear();
            elems.reserve(nE);
            for (std::size_t i = 0; i < nE; i++) {
                Geometry::ElemId id;
                f_in >> id;
                progress_.advance();
                elems.add(mesh_->elems().getId(id));
            }
        } else if (label.compare("End of Planewave")==0) {
            switch (definitionMode) {
            case DefinitionMode::byVectors:
                return new Source::PlaneWave(mag, elems, dir, pol);
            case DefinitionMode::byAngles:
                return new Source::PlaneWave(mag, elems, theta,phi, alpha,beta);
            }

        }
    }
    throw std::logic_error("End of Planewave label not found.");
}

Source::Dipole* Parser::readDipole() {
    //    Geometry::Element::Group<Vol> elems;
    //    Math::Real length = 0.0;
    //    Math::CVecR3 orientation;
    //    Math::CVecR3 position;
    //    MagnitudeGaussian* mag = NULL;
    //
    //    std::string line;
    //    bool finished = false;
    //    char* pEnd;
    //    while(!finished && !f_in.eof()) {
    //        getline(f_in, line);
    //        if (line.find("End of puntual excitation") == line.npos) {
    //            Geometry::ElemId id = Geometry::ElemId(strtol(line.c_str(), &pEnd, 10));
    //            //            Volume<>* elem = mesh_->elems().get(id);
    //            //            elems.add(elem);
    //        } else
    //            finished = true;
    //    }
    //    if (!finished) {
    //        throw std::logic_error("End of excitation type label not found.");
    //    }
    //    //
    //    return new Dipole(mag, elems, length, orientation, position);
    return NULL;
}

Source::Port::Waveguide* Parser::readPortWaveguide() {
    std::size_t numElements = 0;
    Source::Magnitude::Magnitude* mag;
    WaveportShape shape = WaveportShape::rectangular;
    Source::Port::Waveguide::ExcitationMode excitationMode = 
        Source::Port::Waveguide::TE;
    std::pair<Math::UInt,Math::UInt> mode(1,0);
    std::string line, label, value;
    Geometry::Element::Group<const Geometry::Surf> surfs;
    bool finished = false;
    while (!finished && !f_in.eof()) {
        getNextLabelAndValue(label,value);
        if (!label.compare("Shape")) {
            if (value.find("Rectangular") != value.npos) {
                shape = WaveportShape::rectangular;
            } else {
                throw std::logic_error("Unreckognized waveguide port shape.");
            }
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("ExcitationMode") == 0) {
            if (value.find("TE") != value.npos) {
                excitationMode = Source::Port::Waveguide::TE;
            } else if (value.find("TM") != value.npos) {
                excitationMode = Source::Port::Waveguide::TM;
            }
        } else if (label.compare("FirstMode") == 0) {
            mode.first = atoi(value.c_str());
        } else if (label.compare("SecondMode") == 0) {
            mode.second = atoi(value.c_str());
        } else if (label.compare("Number of elements") == 0) {
            numElements = atoi(value.c_str());
        } else if (label.compare("Elements") == 0) {
            std::size_t e, f;
            std::vector<Geometry::Element::Face> faces;
            for (std::size_t i = 0; i < numElements; i++) {
                f_in >> e >> f;
                progress_.advance();
                const Geometry::ElemR* elem = 
                    mesh_->elems().getId(Geometry::ElemId(e));
                if (elem->is<Geometry::VolR>()) {
                    const Geometry::VolR* vol =
                        elem->castTo<Geometry::VolR>();
                    faces.push_back(Geometry::Element::Face(vol, f - 1));
                } else if (elem->is<Geometry::SurfR>()) {
                    surfs.add(elem->castTo<Geometry::SurfR>());
                }
            }
            surfs.add(mesh_->getSurfsMatching(faces));
            if (surfs.size() < faces.size()) {
                surfs.printInfo();
                throw std::logic_error("Could not find surfaces matching element faces.");
            }
            if (surfs.size() == 0) {
                throw std::logic_error("No surfaces read on waveguide port.");
            }
        } else if (label.find("End of Waveguide port") != label.npos) {
            finished = true;
        }
        if (f_in.eof()) {
            throw std::logic_error("End of Waveguide port not found");
        }
    }
    // Throws error message if finished was not updated.
    if (!finished) {
        throw std::logic_error("End of excitation type label not found.");
    }

    Source::Port::Bound3 boundTerminations;
    // X bounds
    if (settings_("Mesher options")("Lower x bound").isString()) {
        boundTerminations[0][0] = strToBoundType(
                settings_("Mesher options")("Lower x bound").getString());
    } else {
        boundTerminations[0][0] = new PhysicalModel::Bound::PML(MatId(0));
    }
    if (settings_("Mesher options")("Upper x bound").isString()) {
        boundTerminations[0][1] = strToBoundType(
                    settings_("Mesher options")("Upper x bound").getString());
    } else {
        boundTerminations[0][1] = new PhysicalModel::Bound::PML(MatId(0));
    }
    // Y bounds
    if (settings_("Mesher options")("Lower y bound").isString()) {
        boundTerminations[1][0] = strToBoundType(
                    settings_("Mesher options")("Lower y bound").getString());
    } else {
        boundTerminations[1][0] = new PhysicalModel::Bound::PML(MatId(0));
    }
    if (settings_("Mesher options")("Upper y bound").isString()) {
        boundTerminations[1][1] = strToBoundType(
                    settings_("Mesher options")("Upper y bound").getString());
    } else {
        boundTerminations[1][1] = new PhysicalModel::Bound::PML(MatId(0));
    }
    // Z bounds
    if (settings_("Mesher options")("Lower z bound").isString()) {
        boundTerminations[2][0] = strToBoundType(
                    settings_("Mesher options")("Lower z bound").getString());
    } else {
        boundTerminations[2][0] = new PhysicalModel::Bound::PML(MatId(0));
    }
    if (settings_("Mesher options")("Upper z bound").isString()) {
        boundTerminations[2][1] = strToBoundType(
                    settings_("Mesher options")("Upper z bound").getString());
    } else {
        boundTerminations[2][1] = new PhysicalModel::Bound::PML(MatId(0));
    }

    // Waveport shape.
    if (shape == WaveportShape::rectangular) {
        return new Source::Port::WaveguideRectangular(mag, surfs,
                excitationMode, mode, boundTerminations);
    } else {
        throw std::logic_error("Unsupported Waveport shape.");
    }
}

Source::Port::TEM* Parser::readPortTEM() {
    std::size_t numElements = 0;
    Source::Magnitude::Magnitude* mag;
    Source::Port::TEM::ExcitationMode excitationMode = Source::Port::TEM::voltage;
    Math::CVecR3 origin;
    Math::Real innerRadius, outerRadius;
    std::string line, label, value;
    Geometry::Element::Group<const Geometry::Surf> surfs;
    bool finished = false;
    while (!finished && !f_in.eof()) {
        getNextLabelAndValue(label,value);
        if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (!label.compare("ExcitationMode")) {
            if (value.find("Voltage") != value.npos) {
                excitationMode = Source::Port::TEM::voltage;
            } else if (value.find("Current") != value.npos) {
                excitationMode = Source::Port::TEM::current;
            }
        } else if (!label.compare("Origin")) {
            origin = strToCVecR3(value);
        } else if (!label.compare("Inner radius")) {
            innerRadius = atof(value.c_str());
        } else if (!label.compare("Outer radius")) {
            outerRadius = atof(value.c_str());
        } else if (!label.compare("Number of elements")) {
            numElements = atoi(value.c_str());
        } else if (!label.compare("Elements")) {
            std::size_t e, f;
            std::vector<Geometry::Element::Face> faces;
            for (std::size_t i = 0; i < numElements; i++) {
                f_in >> e >> f;
                progress_.advance();
                const Geometry::ElemR* elem = 
                    mesh_->elems().getId(Geometry::ElemId(e));
                if (elem->is<Geometry::VolR>()) {
                    const Geometry::VolR* vol = elem->castTo<Geometry::VolR>();
                    faces.push_back(Geometry::Element::Face(vol,f-1));
                } else if (elem->is<Geometry::SurfR>()) {
                    surfs.add(elem->castTo<Geometry::SurfR>());
                }
            }
            surfs.add(mesh_->getSurfsMatching(faces));
            if (surfs.size() < faces.size()) {
                surfs.printInfo();
                throw std::logic_error("Could not find surfaces matching element faces.");
            }
            if (surfs.size() == 0) {
                throw std::logic_error("No surfaces read on TEM port.");
            }
        } else if (label.find("End of TEM port") != label.npos) {
            finished = true;
        }
        if (f_in.eof()) {
            throw std::logic_error("End of TEM port not found");
        }
    }
    // Throws error message if finished was not updated.
    if (!finished) {
        throw std::logic_error("End of excitation type label not found.");
    }
    return new Source::Port::TEMCoaxial(mag, surfs, excitationMode, origin,
            innerRadius, outerRadius);
}

Source::Generator* Parser::readGenerator() {
    Geometry::Element::Group<Geometry::Nod> elems;
    Source::Magnitude::Magnitude* mag;
    Source::Generator::Type type;
    Source::Generator::Hardness hardness;
    std::string filename;
    std::string label, value;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Type")==0) {
            type = strToGeneratorType(value);
        } else if (label.compare("Hardness")==0) {
            hardness = strToGeneratorHardness(value);
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Number of elements")==0) {
            std::size_t nE = atoi(value.c_str());
            std::vector<Geometry::ElemId> nodes;
            nodes.reserve(nE);
            for (std::size_t i = 0; i < nE; i++) {
                std::size_t e;
                f_in >> e;
                progress_.advance();
                Geometry::CoordId id = Geometry::CoordId(e);
                const Geometry::CoordR3* coord = mesh_->coords().getId(id);
                Geometry::NodR* node = 
                    new Geometry::NodR(Geometry::ElemId(0), &coord);
                mesh_->elems().addId(node);
                nodes.push_back(node->getId());
            }
            elems = mesh_->elems().getId(nodes);
        } else if (label.compare("End of Generator")==0) {
            return new Source::Generator(mag, elems, type, hardness);
        }
    }
    throw std::logic_error("End of Source::Generator label not found.");
}

Source::OnLine* Parser::readSourceOnLine() {
    Source::OnLine::Type type;
    Source::OnLine::Hardness hardness;
    Source::Magnitude::Magnitude* mag;
    std::vector<Geometry::ElemId> ids;
    std::string filename;
    std::string label, value;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Type")==0) {
            type = strToNodalType(value);
        } else if (label.compare("Hardness")==0) {
            hardness = strToNodalHardness(value);
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Number of elements")==0) {
            std::size_t nE = atoi(value.c_str());
            ids.reserve(ids.size() + nE);
            for (size_t i = 0; i < nE; i++) {
                size_t e;
                f_in >> e;
                progress_.advance();
                ids.push_back(Geometry::ElemId(e));
            }
        } else if (label.compare("End of Source_on_line")==0) {
            Geometry::Element::Group<Geometry::Lin> lines = 
                mesh_->elems().getId(ids);
            return new Source::OnLine(mag, lines, type, hardness);
        }
    }
    throw std::logic_error("End of Nodal label not found.");
}

OutputRequest::Base::Type Parser::strToOutputType(std::string str) {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return OutputRequest::Base::electric;
    } else if (str.compare("magneticField")==0) {
        return OutputRequest::Base::magnetic;
    } else if (str.compare("electricFieldNormals")==0) {
        return OutputRequest::Base::electricFieldNormals;
    } else if (str.compare("magneticFieldNormals")==0) {
        return OutputRequest::Base::magneticFieldNormals;
        //	} else if (str.compare("powerDensity")==0) {
        //		return OutputRequest::powerDensity;
        //	} else if (str.compare("power")==0) {
        //		return OutputRequest::power;
    } else if (str.compare("current")==0) {
        return OutputRequest::Base::current;
    } else if (str.compare("voltage")==0) {
        return OutputRequest::Base::voltage;
    } else if (str.compare("bulkCurrentElectric")==0) {
        return OutputRequest::Base::bulkCurrentElectric;
    } else if (str.compare("bulkCurrentMagnetic")==0) {
        return OutputRequest::Base::bulkCurrentMagnetic;
    } else if (str.compare("farField")==0) {
        return OutputRequest::Base::electric;
    } else {
        throw std::logic_error("Unrecognized output type: " + str);
    }
}

Parser::SIBCType Parser::strToSIBCType(std::string str) {
    str = trim(str);
    if (str.compare("File")==0) {
        return sibc;
    } else if (str.compare("Layers")==0) {
        return multilayer;
    } else {
        throw std::logic_error("Unrecognized SIBC type: " + str);
    }
}

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
    } else if (str.find("Conn_") != std::string::npos) {
        return PhysicalModel::PhysicalModel::multiport;
    } else if (str.find("Thin_gap")==0) {
        return PhysicalModel::PhysicalModel::gap;
    } else {
        throw std::logic_error("Unrecognized material label: " + str);
    }
}

PhysicalModel::Multiport::Multiport::Type Parser::strToMultiportType(std::string str) {
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

std::string Parser::readVersion() {
    std::string line, label, value;
    bool formatFound = false;
    bool versionFound = false;
    std::string format, version;
    while ((!formatFound || !versionFound) && !f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Format") == 0) {
            formatFound = true;
            format = trim(value);
        }
        if (label.compare("Version") == 0) {
            versionFound = true;
            version = trim(value);
        }
    }
    // Shows error messages.
    if (!formatFound) {
        throw std::logic_error(
                "EoF was reached but format label was not found.");
    }
    if (!versionFound) {
        throw std::logic_error(
                "EoF was reached but version label was not found.");
    }
    return version;
}

Parser::GiDOutputType Parser::strToGidOutputType(std::string str) {
    str = trim(str);
    if (str.compare("OutRq_on_point")==0) {
        return Parser::outRqOnPoint;
    } else if (str.compare("OutRq_on_line")==0) {
        return Parser::outRqOnLine;
    } else if (str.compare("OutRq_on_surface")==0) {
        return Parser::outRqOnSurface;
    } else if (str.compare("OutRq_on_volume")==0) {
        return Parser::outRqOnVolume;
    } else if (str.compare("Bulk_current_on_surface")==0) {
        return Parser::bulkCurrentOnSurface;
    } else if (str.compare("Bulk_current_on_volume")==0) {
        return Parser::bulkCurrentOnVolume;
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
    } else {
        throw std::logic_error("Unrecognized label " + str);
        return Parser::byVectors;
    }
}

OutputRequest::Domain Parser::strToDomain(std::string line) {
    std::size_t timeDomain;
    Math::Real initialTime;
    Math::Real finalTime;
    Math::Real samplingPeriod;
    std::size_t frequencyDomain;
    Math::Real initialFrequency;
    Math::Real finalFrequency;
    Math::Real frequencyStep;
    std::size_t logFrequencySweep;
    std::size_t usingTransferFunction;
    std::string transferFunctionFile;
    std::stringstream ss(line);
    ss >> timeDomain >> initialTime >> finalTime >> samplingPeriod
       >> frequencyDomain >> initialFrequency >> finalFrequency >>frequencyStep
       >> logFrequencySweep >> usingTransferFunction >> transferFunctionFile;
    //
    transferFunctionFile.erase(std::remove( transferFunctionFile.begin(),
            transferFunctionFile.end(), '\"' ),transferFunctionFile.end());
    //
    return
        OutputRequest::Domain(
            toBool(timeDomain), initialTime, finalTime, samplingPeriod,
            toBool(frequencyDomain), initialFrequency, finalFrequency,
            frequencyStep,	toBool(logFrequencySweep),
            toBool(usingTransferFunction), transferFunctionFile);
}

Source::Magnitude::Magnitude* Parser::readMagnitude(const std::string typeIn) {
    std::string type = typeIn;
    type = trim(type);
    bool finished = false;
    std::string label, value;
    if (type.compare("Gaussian") == 0) {
        Math::Real delay, spread;
        bool spreadFound = false;
        bool delayFound = false;
        while (!finished && !f_in.eof()) {
            getNextLabelAndValue(label, value);
            if (label.compare("Gaussian spread") == 0) {
                spread = atof(value.c_str());
                spreadFound = true;
            } else if (label.compare("Gaussian delay") == 0) {
                delay = atof(value.c_str());
                delayFound = true;
            }
            finished = spreadFound && delayFound;
            if (finished) {
                return new Source::Magnitude::Magnitude(
                        new Math::Function::Gaussian(spread, delay));
            }
        }
    } else if (type.compare("File") == 0) {
        std::string excName;
        while (!finished && !f_in.eof()) {
            getNextLabelAndValue(label, value);
            if (label.compare("Filename") == 0) {
                excName = trim(value);
                finished = true;
            }
            if (finished) {
                return new Source::Magnitude::Numerical(getFolder() + excName);
            }
        }
    }
    throw std::logic_error(
            "Unable to recognize magnitude type when reading excitation.");
}

bool Parser::checkVersionCompatibility(const std::string version) const {
    bool versionMatches =
            atof(version.c_str()) == atof(std::string(APP_VERSION).c_str());
    if (!versionMatches) {
        throw std::logic_error(
                "File version " + version + " is not supported.");
    }
    return versionMatches;
}

PhysicalModel::Volume::PoleResidue Parser::readPoleResiduePair(std::ifstream& stream) {
    std::string line;
    std::getline(stream, line);
    std::size_t prev = 0, pos;
    std::vector<std::string> wordVector;
    while ((pos = line.find_first_of(" (,)", prev)) != std::string::npos) {
        if (pos > prev) {
            wordVector.push_back(line.substr(prev, pos-prev));
        }
        prev = pos+1;
    }
    if (prev < line.length()) {
        wordVector.push_back(line.substr(prev, std::string::npos));
    }
    assert(wordVector.size() == 4);
    PhysicalModel::Volume::PoleResidue res;
    res.first = std::complex<Math::Real>(
                    atof(wordVector[0].c_str()),
                    atof(wordVector[1].c_str()));
    res.second = std::complex<Math::Real>(
                    atof(wordVector[2].c_str()),
                    atof(wordVector[3].c_str()));
    return res;
}

PhysicalModel::Volume::Anisotropic::Model Parser::strToAnisotropicModel(
        std::string label) {
    std::string str = label;
    str = trim(str);
    if (str.compare("Crystal")==0) {
        return PhysicalModel::Volume::Anisotropic::Model::crystal;
    } else if (str.compare("Ferrite")==0) {
        return PhysicalModel::Volume::Anisotropic::Model::ferrite;
    } else {
        throw std::logic_error("Unrecognized Anisotropic Model: " + str);
    }
}

Math::Axis::Local Parser::strToLocalAxes(const std::string& str) {
    std::size_t begin = str.find_first_of("{");
    std::size_t end = str.find_first_of("}");
    Math::CVecR3 eulerAngles = strToCVecR3(str.substr(begin+1,end-1));
    begin = str.find_last_of("{");
    end = str.find_last_of("}");
    Math::CVecR3 origin = strToCVecR3(str.substr(begin+1,end-1));
    return Math::Axis::Local(eulerAngles, origin);
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

void Parser::getline_(std::string& line) {
    getline(f_in, line);
    progress_.advance();
}

} /* namespace GiD */
} /* namespace Parser */
} /* namespace SEMBA */
