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
/*
 * ParserGiD.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#include "ParserGiD.h"

ParserGiD::ParserGiD() {
    mesh_ = NULL;
}

ParserGiD::ParserGiD(const string& fn)
:   ProjectFile(fn) {
    mesh_ = NULL;
}

ParserGiD::~ParserGiD() {

}

SmbData* ParserGiD::read() {
    if (isFolder()) {
        string msg(getFilename() +  " is a directory.");
        throw Error(msg);
    }
    if (!canOpen()) {
        throw Error("Can not openfile: " + getFilename());
    }
    openAsInput(f_in);
    string version = readVersion();
    if (!checkVersionCompatibility(version)) {
        throw Error("File version " + version + " is not supported.");
    }

    SmbData* res = new SmbData();
    res->setFilename(getFilename());
    res->solverOptions = readOptionsSolver();
    res->mesherOptions = readMesherOptions();
    pSize_ = readProblemSize();
    res->pMGroup = readMaterials();
    mesh_ = readMesh();
    res->mesh = mesh_;
    res->grid = readCartesianGrid();
    res->emSources = readEMSources();
    res->outputRequests = readOutputRequests();

    postReadOperations(res);

    return res;
}

void ParserGiD::printInfo() const {
    cout << "--- GiDParser info ---" << endl;
    cout << "--- End of GiDParser info ---" << endl;
}

OptionsSolver* ParserGiD::readOptionsSolver() {
    OptionsSolver base;
    bool finished = false;
    bool optionsFound = false;
    while (!optionsFound && !f_in.eof()) {
        string label, value;
        getNextLabelAndValue(label, value);
        if (label.compare("Solver options") == 0) {
            optionsFound = true;
            while (!finished && !f_in.eof() ) {
                getNextLabelAndValue(label, value);
                if (label.compare("Final time") == 0) {
                    base.setFinalTime(atof(value.c_str()));
                } else if (label.compare("Number of time steps") == 0) {
                    base.setNumberOfTimeSteps(atoi(value.c_str()));
                } else if (label.compare("Time step") == 0) {
                    base.setTimeStep(atof(value.c_str()));
                } else if (label.compare("CFL") == 0) {
                    base.setCFL(atof(value.c_str()));
                } else if (label.compare("Default sampling period") == 0) {
                    base.setSamplingPeriod(atof(value.c_str()));
                } else if (label.compare("Force restarting") == 0) {
                    base.setForceRestarting(strToBool(value));
                } else if (label.compare("Resume simulation") == 0) {
                    base.setResumeSimulation(strToBool(value));
                } else if (label.compare("Flush") == 0) {
                    base.setFlush(atof(value.c_str()));
                } else if (label.compare("Additional arguments") == 0) {
                    base.setAdditionalArguments(value);
                } else if (label.compare("Solver") == 0) {
                    switch (strToSolver(value)) {
                    case OptionsSolver::Solver::ugrfdtd:
                        return readOptionsSolverFDTD(base);
                    case OptionsSolver::Solver::cudg3d:
                        return readOptionsSolverDGTD(base);
                    default:
                        break;
                    }
                } else if(label.find("End of solver options") != label.npos) {
                    finished = true;
                }
            } // Closes ( !finished && !f_in.eof() ) while.
        } // Closes problem data found if.
    } // Closes problemDataFound while.
    // Throws error messages if a problem was detected.
    throw Error("No options were found.");
}

OptionsSolverFDTD* ParserGiD::readOptionsSolverFDTD(
        const OptionsSolver& base) {
    bool finished = false;
    bool optionsFound = false;
    OptionsSolverFDTD* res = new OptionsSolverFDTD(base);
    while (!optionsFound && !f_in.eof()) {
        string label, value;
        getNextLabelAndValue(label, value);
        if (label.compare("ugrfdtd options") == 0) {
            optionsFound = true;
            while (!finished && !f_in.eof() ) {
                if (label.compare("Composites model") == 0) {
                    res->setCompositeModel(strToCompositeModel(value));
                } else if (label.compare("Conformal skin") == 0) {
                    res->setConformalSkin(strToBool(value));
                } else if (label.compare("No compo Mur") == 0) {
                    res->setNoCompoMur(strToBool(value));
                } else if (label.compare("Skin depth") == 0) {
                    res->setSkinDepth(strToBool(value));
                } else if (label.compare("Composites attenuation factor") == 0) {
                    res->setCompositesAttenuationFactor(atof(value.c_str()));
                } else if (label.compare("Wires flavor") == 0) {
                    res->setWireModel(strToWireModel(value));
                } else if (label.compare("MTLN") == 0) {
                    res->setMTLN(strToBool(value));
                } else if (label.compare("Min distance wires") == 0) {
                    res->setMinDistanceWires(atof(value.c_str()));
                } else if (label.compare("New dispersive formulation") == 0) {
                    res->setNewDispersiveFormulation(strToBool(value));
                } else if (label.compare("Taparrabos") == 0) {
                    res->setTaparrabos(strToBool(value));
                } else if (label.compare("Make holes") == 0) {
                    res->setMakeHoles(strToBool(value));
                } else if (label.compare("Ground wires") == 0) {
                    res->setGroundWires(strToBool(value));
                } else if (label.compare("Inductance model") == 0) {
                    res->setInductanceModel(strToInductanceModel(value));
                } else if (label.compare("Wires attenuation factor") == 0) {
                    res->setWiresAttenuationFactor(atof(value.c_str()));
                } else if (label.compare("Use default PML") == 0) {
                    res->setUseDefaultPml(strToBool(value.c_str()));
                } else if (label.compare("PML alpha factor") == 0) {
                    Real factor = atof(value.c_str());
                    getNextLabelAndValue(label, value);
                    Real order = atof(value.c_str());
                    res->setPMLAlpha(pair<Real,Real>(factor, order));
                } else if (label.compare("PML kappa") == 0) {
                    res->setPMLKappa(atof(value.c_str()));
                } else if (label.compare("PML correction factor") == 0) {
                    Real factor = atof(value.c_str());
                    getNextLabelAndValue(label, value);
                    Real depth = atof(value.c_str());
                    res->setPMLCorrection(pair<Real,Real>(factor, depth));
                } else if (label.compare("PML backing") == 0) {
                    res->setPMLBacking(strToPMLBacking(value));
                } else if (label.compare("Map") == 0) {
                    res->setMap(strToBool(value));
                } else if (label.compare("Map VTK") == 0) {
                    res->setMapVtk(strToBool(value));
                } else if (label.compare("No NF2FF") == 0) {
                    res->setNoNF2FF(strToNoNF2FF(value));
                } else if (label.compare("NF2F decimation") == 0) {
                    res->setNF2FFDecimation(strToBool(value));
                } else if(label.find("End of ugrfdtd options") != label.npos) {
                    finished = true;
                    return res;
                }
            } // Closes ( !finished && !f_in.eof() ) while.
        } // Closes problem data found if.
    } // Closes problemDataFound while.
    // Throws error messages if a problem was detected.
    throw Error("No options were found.");
    }

OptionsSolverDGTD* ParserGiD::readOptionsSolverDGTD(
        const OptionsSolver& base) {
    bool optionsFound = false;
    OptionsSolverDGTD* res = new OptionsSolverDGTD(base);
    while (!optionsFound && !f_in.eof()) {
        string label, value;
        getNextLabelAndValue(label, value);
        if (label.compare("cudg3d options") == 0) {
            optionsFound = true;
            while (!f_in.eof()) {
                if (label.compare("Upwinding") == 0) {
                    res->setUpwinding(atof(value.c_str()));
                } else if (label.compare("Time integrator") == 0) {
                    res->setTimeIntegrator(
                            OptionsSolverDGTD::strToTimeIntegrator(trim(value)));
                } else if (label.compare("Use LTS") == 0) {
                    res->setUseLTS(strToBool(value));
                } else if (label.compare("Grow smaller tiers") == 0) {
                    res->setGrowSmallerTiers(atoi(value.c_str()));
                } else if (label.compare("Max number of tiers") == 0) {
                    res->setMaxNumberOfTiers(atoi(value.c_str()));
                } else if (label.compare("Use max stage size for LTS") == 0) {
                    res->setUseMaxStageSizeForLTS(strToBool(value));
                } else if (label.compare("PML constant conductivity profile") == 0) {
                    res->setPMLConstantConductivityProfile(strToBool(value));
                } else if (label.compare("PML conductivity") == 0) {
                    res->setPMLConductivity(atof(value.c_str()));
                } else if(label.find("End of cudg3d options") != label.npos) {
    				return res;
                }
            } // Closes ( !finished && !f_in.eof() ) while.
        } // Closes problem data found if.
    } // Closes problemDataFound while.
    // Throws error messages if a problem was detected.
    throw Error("No options were found.");
}

OptionsMesher* ParserGiD::readMesherOptions() {
    bool finished;
    bool found = false;
    string line, label, value;
    OptionsMesher* res = new OptionsMesher();
    bool paddingByNumberOfCells = false;
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Mesher options") == 0) {
            found = true;
            finished = false;
            while (!finished && !f_in.eof() ) {
                getNextLabelAndValue(label, value);
                if (label.compare("Mesher") == 0) {
                    res->setMesher(strToMesher(value));
                } else if (label.compare("Brute force volumes") == 0) {
                    res->setBruteForceVolumes(strToBool(value));
                } else if (label.compare("VTK Export") == 0) {
                    res->setVtkExport(strToBool(value));
                } else if (label.compare("postmsh Export") == 0) {
                    res->setPostmshExport(strToBool(value));
                } else if (label.compare("Mode") == 0) {
                    res->setMode(strToMesherMode(value));
                } else if (label.compare("Forbidden length") == 0) {
                    res->setForbiddenLength(atof(trim(value).c_str()));
                } else if (label.compare("Scale factor") == 0) {
                    res->setScaleFactor(strToBool(value));
                } else if (label.compare("Scale factor value") == 0) {
                    res->setScaleFactorValue(trim(value));
                } else if (label.compare("Location in mesh")==0) {
                    CVecR3 location = strToCVecR3(trim(value));
                    res->setLocationInMesh(location);
                } else if (label.compare("Geometry scaling factor") == 0) {
                    scalingFactor_ = atof(value.c_str());
                } else if (label.compare("Upper x bound") == 0) {
                    res->setBoundTermination(x,1,strToBoundType(value));
                } else if (label.compare("Lower x bound") == 0) {
                    res->setBoundTermination(x,0,strToBoundType(value));
                } else if (label.compare("Upper y bound") == 0) {
                    res->setBoundTermination(y,1,strToBoundType(value));
                } else if (label.compare("Lower y bound") == 0) {
                    res->setBoundTermination(y,0,strToBoundType(value));
                } else if (label.compare("Upper z bound") == 0) {
                    res->setBoundTermination(z,1,strToBoundType(value));
                } else if (label.compare("Lower z bound") == 0) {
                    res->setBoundTermination(z,0,strToBoundType(value));
                } else if (label.compare("Boundary padding type") == 0) {
                    if (trim(value).compare("by_number_of_cells")==0) {
                        paddingByNumberOfCells = true;
                    } else {
                        paddingByNumberOfCells = false;
                    }
                } else if (label.compare("Boundary padding") == 0) {
                    boundaryPadding_ = strToBound(value);
                } else if (label.compare("Boundary mesh size") == 0) {
                    boundaryMeshSize_ = strToBound(value);
                } else if (label.compare("End of mesher options")==0) {
                    finished = true;
                }
            }
        }
    }

    if (paddingByNumberOfCells) {
        boundaryPadding_.first =
                boundaryPadding_.first * boundaryMeshSize_.first;
        boundaryPadding_.second =
                boundaryPadding_.second * boundaryMeshSize_.second;
    }

    return res;
}

MeshUnstructured* ParserGiD::readMesh() {
    GroupLayers<> lG = readLayers();
    GroupCoordinates<CoordR3> cG = readCoordinates();
    GroupElements<ElemR> eG = readElements(cG);
    return new MeshUnstructured(cG, eG, lG);
}

GroupEMSources<>* ParserGiD::readEMSources() {
    GroupEMSources<>* res = new GroupEMSources<>();
    bool finished = false;
    bool found = false;
    string label, value;
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
        throw Error("Excitations label was not found.");
    }
    //
    return res;
}

GroupPhysicalModels<>* ParserGiD::readMaterials(){
    GroupPhysicalModels<>* res = new GroupPhysicalModels<>();
    string label, value;
    UInt materialCount = 0;
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

PhysicalModel* ParserGiD::readPhysicalModel(const MatId id) {
    string name;
    PhysicalModel::Type type;
    PMMultiport::Type mpType = PMMultiport::undefined;
    SIBCType surfType = undefinedSIBC;
    string layersStr;
    Real rEps, rMu, eC, mC;
    PMVolumeAnisotropic::Model anisotropicModel;
    Real crystalRMu, ferriteREps, ferriteRMu;
    CVecR3 rEpsPrincipalAxes;
    LocalAxes localAxes;
    Real kappa;
    Real radius, R, L, C;
    ProjectFile file;
    while (!f_in.eof()) {
        string label, value;
        getNextLabelAndValue(label, value);
        if (label.compare("Name")==0) {
            name = trim(value);
        } else if (label.compare("TypeId")==0) {
            type = strToMaterialType(value);
            if (type == PhysicalModel::multiport) {
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
        } else if (label.compare("Radius")==0) {
            radius = atof(value.c_str());
        } else if (label.compare("Resistance")==0) {
            R = atof(value.c_str());
        } else if (label.compare("Inductance")==0) {
            L = atof(value.c_str());
        } else if (label.compare("Capacitance")==0) {
            C = atof(value.c_str());
        } else if (label.compare("SurfaceType")==0) {
            surfType = strToSIBCType(value);
        } else if (label.compare("Filename")==0) {
            file = ProjectFile(getFolder() + trim(value));
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
            case PhysicalModel::PEC:
                return new PMPEC(id, name);
            case PhysicalModel::PMC:
                return new PMPMC(id, name);
            case PhysicalModel::SMA:
                return new PMSMA(id, name);
            case PhysicalModel::PML:
                return new PMVolumePML(id, name);
            case PhysicalModel::classic:
                return new PMVolumeClassic(id, name, rEps, rMu, eC, mC);
            case PhysicalModel::elecDispersive:
                return new PMVolumeDispersive(id, name, file);
            case PhysicalModel::anisotropic:
                switch (anisotropicModel) {
                case PMVolumeAnisotropic::Model::crystal:
                    return new PMVolumeAnisotropicCrystal(id, name, localAxes,
                            rEpsPrincipalAxes, crystalRMu);
                case PMVolumeAnisotropic::Model::ferrite:
                    return new PMVolumeAnisotropicFerrite(id, name, localAxes,
                            kappa,ferriteRMu,ferriteREps);
                default:
                    throw Error("Material type not recognized.");
                }
                case PhysicalModel::isotropicsibc:
                    switch (surfType) {
                    case sibc:
                        return new PMSurfaceSIBC(id, name, file);
                    case multilayer:
                        return readMultilayerSurf(id, name, layersStr);
                    default:
                        throw Error("Undefined SIBC Type.");
                    }
                    break;
                    case PhysicalModel::wire:
                        return new PMWire(id, name, radius, R, L);
                    case PhysicalModel::multiport:
                        if (mpType == PMMultiport::shortCircuit) {
                            return new PMMultiportPredefined(id, name, mpType);
                        } else {
                            return new PMMultiportRLC(id, name, mpType, R, L, C);
                        }
                    default:
                        throw Error("Material type not recognized.");
            }
        }
    }
    return NULL;
}

GroupOutRqs<>* ParserGiD::readOutputRequests() {
    GroupOutRqs<>* res = new GroupOutRqs<>();
    bool finished;
    bool found = false;
    string line, label, value;
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

GroupElements<Vol> ParserGiD::boundToElemGroup(const string& line) {
    BoxR3 bound = strToBound(line);
    HexR8* hex = new HexR8(*mesh_, ElementId(0), bound);
    mesh_->elems().addId(hex);
    GroupElements<Vol> elems(hex);
    return elems;
}

void ParserGiD::readOutRqInstances(GroupOutRqs<>* res) {
    bool finished = false;
    GiDOutputType gidOutputType;
    while (!finished && !f_in.eof()) {
        string line, label, value;
        getNextLabelAndValue(label,value);
        if (label.compare("GiDOutputType")==0) {
            gidOutputType = strToGidOutputType(trim(value));
        } else if (label.compare("Number of elements")==0) {
            UInt nE = atoi(value.c_str());
            for (UInt i = 0; i < nE; i++) {
                getNextLabelAndValue(label,value);
                string name = trim(value);
                getNextLabelAndValue(label,value);
                OutRqBase::Type type = strToOutputType(trim(value));
                getNextLabelAndValue(label,value);
                Domain domain = strToDomain(value);
                switch (gidOutputType) {
                case ParserGiD::outRqOnPoint:
                {
                    getNextLabelAndValue(label,value);
                    CoordinateId coordId(atoi(value.c_str()));
                    NodR* node = new NodR(*mesh_, ElementId(0), &coordId);
                    mesh_->elems().addId(node);
                    GroupElements<Nod> elems(node);
                    res->add(new OutRq<Nod>(domain, type, name, elems));
                    break;
                }
                //case ParserGiD::outRqOnLine:
                //    getNextLabelAndValue(label,value);
                //    elem.clear();
                //    elem.push_back(ElementId(atoi(value.c_str())));
                //    res->add(new OutRq(elem, domain, type, name));
                //    break;
                case ParserGiD::outRqOnSurface:
                {
                    getNextLabelAndValue(label,value);
                    vector<ElementId> ids;
                    ids.push_back(ElementId(atoi(value.c_str())));
                    GroupElements<ElemR> elems = mesh_->elems().getId(ids);
                    GroupElements<Surf> surfs = elems.getOf<Surf>();
                    res->add(new OutRq<Surf>(domain, type, name, surfs));
                    break;
                }
                case ParserGiD::outRqOnVolume:
                {
                    getline(f_in, line);
                    GroupElements<Vol> elems = boundToElemGroup(line);
                    res->add(new OutRq<Vol>(domain, type, name, elems));
                    break;
                }
                case ParserGiD::bulkCurrentOnSurface:
                {
                    CartesianAxis dir;
                    UInt skip;
                    getNextLabelAndValue(label,value);
                    switch (value[0]) {
                    case 'x':
                        dir = x;
                        break;
                    case 'y':
                        dir = y;
                        break;
                    case 'z':
                        dir = z;
                        break;
                    default:
                        dir = x;
                    }
                    getNextLabelAndValue(label,value);
                    skip = atoi(value.c_str());
                    getline(f_in, line);
                    vector<ElementId> ids;
                    ids.push_back(ElementId(atoi(line.c_str())));
                    GroupElements<ElemR> elems = mesh_->elems().getId(ids);
                    GroupElements<Surf> surfs = elems.getOf<Surf>();
                    res->add(new OutRqBulkCurrent(domain, name, surfs,
                            dir, skip));
                    break;
                }
                case ParserGiD::bulkCurrentOnVolume:
                {
                    CartesianAxis dir;
                    UInt skip;
                    getNextLabelAndValue(label,value);
                    switch (value[0]) {
                    case 'x':
                        dir = x;
                        break;
                    case 'y':
                        dir = y;
                        break;
                    case 'z':
                        dir = z;
                        break;
                    default:
                        dir = x;
                    }
                    getNextLabelAndValue(label,value);
                    skip = atoi(value.c_str());
                    getline(f_in, line);
                    GroupElements<Vol> elems = boundToElemGroup(line);
                    res->add(new OutRqBulkCurrent(domain, name, elems,
                            dir, skip));
                    break;
                }
                case ParserGiD::farField:
                {
                    getline(f_in, line);
                    GroupElements<Vol> elems = boundToElemGroup(line);
                    Real iTh, fTh, sTh, iPhi, fPhi, sPhi;
                    f_in >> iTh >> fTh >> sTh >> iPhi >> fPhi >> sPhi;
                    getline(f_in, line);
                    OutRqFarField* oRFF = new OutRqFarField(domain, name, elems,
                            iTh, fTh, sTh, iPhi, fPhi, sPhi);
                    res->add(oRFF);
                    break;
                }
                default:
                    cerr << endl << "ERROR @ GiDParser: "
                    << "Unrecognized GiD Output request type:"
                    << type << endl;
                    break;
                }
            } // End of loop running over the elements.
        } else if (label.compare("End of Output request instance")==0) {
            finished = true;
        } else {
            cerr << endl << "ERROR @ GiDParser::readOutputRequestsInstance(): "
                    << "Label not identified: " << label << endl;
        } // End of condition comparing labels.
    }
}

ProblemSize
ParserGiD::readProblemSize() {
    string label, line, value;
    bool finished = false;
    bool problemSizeFound = false;
    ProblemSize res;
    // Runs until "Problem Size" is found or eof is reached.
    while (!problemSizeFound && !f_in.eof()) {
        getline(f_in, line);
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
                    cerr << endl << "ERROR @ Parser::readProblemSize(): "
                            << "End of problem size label not found" << endl;
                }
            }
        }
    }
    if (!problemSizeFound) {
        cerr << endl << "ERROR @ Parser::readProblemSize: "
                << "Problem size label not found."     << endl;
    }
    return res;
}

GroupLayers<> ParserGiD::readLayers() {
    bool finished = false;
    bool found = false;
    string label, value;
    LayerId id;
    GroupLayers<> res;
    while (!found && !f_in.eof() ) {
        getNextLabelAndValue(label, value);
        if (label.compare("Layers")==0) {
            found = true;
            while(!finished && !f_in.eof()) {
                string line;
                getline(f_in, line);
                if (line.find("End of layers") != line.npos) {
                    finished = true;
                } else {
                    stringstream ss(line);
                    ss >> id >> value;
                    res.add(new Layer(id, value));
                }
            }
        }
    }
    if (!found) {
        cerr << endl << "ERROR @ Parsing layers: "
                << "Layers label was not found." << endl;
        return res;
    }
    return res;
}

GroupCoordinates<CoordR3> ParserGiD::readCoordinates() {
    string line;
    CoordinateId id;
    CVecR3 pos;
    vector<CoordR3*> coord;
    coord.reserve(pSize_.v);
    bool finished = false;
    bool found = false;
    while (!found && !f_in.eof() && !finished) {
        getline(f_in, line);
        if (line.find("Coordinates:") != line.npos) {
            found = true;
            // Reads coordinates.
            for (UInt i = 0; i < pSize_.v; i++) {
                f_in >> id >> pos(0) >> pos(1) >> pos(2);
                coord.push_back(new CoordR3(id, pos));
            }
            // Checks "end of coordinates" label.
            finished = false;
            while(!finished && !f_in.eof()) {
                getline(f_in, line);
                if (line.find("End of coordinates") != line.npos) {
                    finished = true;
                }
            }
        }
    }
    if (!found) {
        throw Error("Coordinates label was not found.");
    }
    if (!finished) {
        throw Error("End of coordinates label not found.");
    }
    return GroupCoordinates<CoordR3>(coord);
}

GroupElements<ElemR> ParserGiD::readElements(
        const GroupCoordinates<CoordR3>& v) {
    string line, label;
    bool finished = false;
    bool found = false;
    vector<ElemR*> elems;
    while (!finished && !f_in.eof()) {
        getline(f_in, line);
        if (line.find("Elements:") != line.npos) {
            found = true;
        }
        // Stores data label in labelStr string.
        label = line.substr(0, line.find(LABEL_ENDING));
        if (label.compare("Linear Hexahedral Elements")==0 ||
                label.compare("Hexahedral Elements")==0) {
            readHex8Elements(v, elems);
        } else if (label.compare("Quadratic Tetrahedral Elements")==0) {
            readTet10Elements(v, elems);
        } else if (label.compare("Linear Tetrahedral Elements")==0 ||
                label.compare("Tetrahedral Elements")==0) {
            readTet4Elements(v, elems);
        } else if (label.compare("Quadratic Triangle Elements")==0) {
            readTri6Elements(v, elems);
        } else if (label.compare("Linear Triangle Elements")==0 ||
                label.compare("Triangle Elements")==0) {
            readTri3Elements(v, elems);
        } else if (label.compare("Linear Line Elements")==0 ||
                label.compare("Line Elements")==0) {
            readLin2Elements(v, elems);
        } else if(label.find("End of Elements") != label.npos) {
            finished = true;
        }
    }
    // Shows error message if the elements label was not found.
    if (!found) {
        cerr << endl << "ERROR @ GiDParser::readElements(): "
                << "\"Elements\" label was not found." << endl;
    }
    // This code is reached only in case of "End of elements" is not found.
    if (!finished) {
        cerr << endl << "ERROR @ GiDParser::readElements()"
                << "\"End of elements\" label was not found." << endl;
    }
    //
    return GroupElements<ElemR>(elems);
}

void ParserGiD::readHex8Elements(const GroupCoordinates<CoordR3>& v,
        vector<ElemR*>& elems) {
    ElementId id;
    CoordinateId vId[8];
    MatId matId;
    for (UInt i = 0; i < pSize_.hex8; i++) {
        f_in >> id;
        for (UInt j = 0; j < 8; j++) {
            f_in >> vId[j];
        }
        f_in >> matId;
        elems.push_back(new HexR8(v, id, vId, LayerId(0), matId));
    }
}

void ParserGiD::readTet10Elements(const GroupCoordinates<CoordR3>& v,
        vector<ElemR*>& elems) {
    ElementId id;
    CoordinateId vId[10];
    MatId matId;
    for (UInt i = 0; i < pSize_.tet10; i++) {
        f_in >> id;
        for (UInt j = 0; j < 10; j++) {
            f_in >> vId[j];
        }
        f_in >> matId;
        elems.push_back(new Tetrahedron10(v, id, vId, LayerId(0), matId));
    }
}

void ParserGiD::readTet4Elements(const GroupCoordinates<CoordR3>& v,
        vector<ElemR*>& elems) {
    ElementId id;
    CoordinateId vId[4];
    LayerId layerId;
    MatId matId;
    for (UInt i = 0; i < pSize_.tet4; i++) {
        f_in >> id >> vId[0] >> vId[1] >> vId[2] >> vId[3] >> matId >> layerId;
        elems.push_back(new Tetrahedron4(v, id, vId, layerId, matId));
    }
}

void ParserGiD::readTri6Elements(const GroupCoordinates<CoordR3>& v,
        vector<ElemR*>& elems) {
    ElementId id;
    CoordinateId vId[6];
    MatId matId;
    CVecR3 normal;
    for (UInt i = 0; i < pSize_.tri6; i++) {
        f_in >> id;
        for (UInt j = 0; j < 6; j++)
            f_in >> vId[j];
        f_in >> matId;
        elems.push_back(new Triangle6(v, id, vId, LayerId(0), matId));
    }
}

void ParserGiD::readTri3Elements(const GroupCoordinates<CoordR3>& v,
        vector<ElemR*>& elems) {
    ElementId id;
    CoordinateId vId[3];
    LayerId layerId;
    MatId matId;
    CVecR3 normal;
    for (UInt i = 0; i < pSize_.tri3; i++) {
        f_in >> id >> vId[0] >> vId[1] >> vId[2] >> matId >> layerId;
        elems.push_back(new Triangle3(v, id, vId, layerId, matId));
    }
}

void ParserGiD::readLin2Elements(const GroupCoordinates<CoordR3>& v,
        vector<ElemR*>& elems) {
    ElementId id;
    CoordinateId vId[2];
    LayerId layerId;
    MatId matId;
    for (UInt i = 0; i < pSize_.lin2; i++) {
        f_in >> id >> vId[0] >> vId[1] >> matId >> layerId;
        elems.push_back(new LinR2(v, id, vId, layerId, matId));
    }
}

PMVolumeDispersive*
ParserGiD::readDispersiveMatFile(
        const MatId id,
        const string& name,
        const ProjectFile& file) const {
    if (!file.canOpen()) {
        throw ErrorFileNotExists(file);
    }
    ifstream stream;
    file.openAsInput(stream);
    Real sig, eps, mu, sigM;
    stream >> sig >> eps >> mu >> sigM;
    UInt nPoles, trash;
    stream >> nPoles >> trash >> trash >> trash;
    vector<PoleResidue> poleResidues;
    for (UInt i = 0; i < nPoles; i++) {
        PoleResidue pR = readPoleResiduePair(stream);
        poleResidues.push_back(pR);
    }
    return new PMVolumeDispersive(id, name,
            eps / Constants::eps0,
            mu / Constants::mu0,
            sig, sigM, poleResidues);
}

PMSurfaceMultilayer*
ParserGiD::readMultilayerSurf(
        const MatId id,
        const string& name,
        const string& layersStr) const {
    UInt begin = layersStr.find_first_of("\"");
    UInt end = layersStr.find_last_of("\"");
    istringstream ss(layersStr.substr(begin+1,end-2));
    string sub;
    vector<Real> thick, rEps, rMu, eCond, mCond;
    UInt parameters;
    string trash;
    ss >> trash >> parameters;
    const UInt nLayers = parameters / 5;
    for (UInt i = 0; i < nLayers; i++) {
        // Thickness, Permittivity, Permeability, ElecCond, MagnCond.
        Real thick_, rEps_, rMu_, eCond_, mCond_;
        ss >> thick_ >> rEps_ >> rMu_ >> eCond_ >> mCond_;
        thick.push_back(thick_);
        rEps.push_back(rEps_);
        rMu.push_back(rMu_);
        eCond.push_back(eCond_);
        mCond.push_back(mCond_);
    }
    return new PMSurfaceMultilayer(id, name, thick, rEps, rMu, eCond, mCond);
}

PMSurfaceSIBC*
ParserGiD::readIsotropicSurfMatFile(
        const MatId id,
        const string& fileName,
        const ProjectFile& file) const {
    ifstream matFile;
    string line, label, value;
    string name, model;
    char *pEnd;
    StaMatrix<Real,2,2> Zstatic, Zinfinite;
    vector<Real> pole;
    vector<StaMatrix<Real,2,2> > Z;
    Real tmpP;
    // Opens file, read only mode.
    matFile.open(fileName.c_str(), ifstream::in);
    if (matFile.fail()) {
        cerr << endl << "ERROR @ readSurfaceMaterialFile(): "
                << "Problem opening file: " << fileName << endl;
    }
    // Parses first line, containing material name.
    getline(matFile, line);
    if (line.find("#PANEL#") == string::npos) {
        cerr << endl << "ERROR @ Parser::readSurfaceMaterialFile(...)"
                << "File: " << fileName << "   "
                << "#PANEL# label has not been found in first line" << endl;
    }
    name = line.substr(8, line.length()-9);
    getline(matFile, line);
    UInt nPoles = 0;
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
    for (UInt i = 0; i < nPoles; i++) {
        MatR22 tmpZ;
        matFile >> tmpP >> tmpZ(0,0) >> tmpZ(0,1) >> tmpZ(1,0) >> tmpZ(1,1);
        pole.push_back(tmpP);
        Z.push_back(tmpZ);
    }
    // Copies all parsed data into the aux material depending on the model.
    return new PMSurfaceSIBC (id, name, Zinfinite, Zstatic, pole, Z);
}

void
ParserGiD::getNextLabelAndValue(string& label, string& value) {
    string line;
    getline(f_in, line);
    line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    label = line.substr(0, line.find(LABEL_ENDING));
    label = trim(label);
    value = line.substr(line.find(LABEL_ENDING) + 1, line.length());
    value = trim(value);
}

Grid3*
ParserGiD::readCartesianGrid() {
    string label, line, value;
    bool finished = false;
    bool gridLabelFound = false;
    bool gridFound = false;
    Grid3* grid = NULL;
    BoxR3 bound;
    bool stepsByNumberOfCells = true;
    CVecI3 numElems;
    CVecR3 steps;
    while (!gridLabelFound && !f_in.eof()) {
        getline(f_in, line);
        if (line.find("Grid:") != line.npos ) {
            gridLabelFound = true;
            while(!finished) {
                getNextLabelAndValue(label, value);
                if (label.compare("Layer Box")==0) {
                    gridFound = true;
                    bound = BoxR3(strToBound(value));
                } else if (label.compare("Type")==0) {
                    if (trim(value).compare("by_number_of_cells")==0) {
                        stepsByNumberOfCells = true;
                    } else {
                        stepsByNumberOfCells = false;
                    }
                } else if (label.compare("Directions")==0) {
                    CVecR3 aux = strToCartesianVector(value);
                    if (stepsByNumberOfCells) {
                        numElems(0) = (Int) aux(0);
                        numElems(1) = (Int) aux(1);
                        numElems(2) = (Int) aux(2);
                    } else {
                        steps = aux;
                    }
                } else if(label.find("End of Grid") != label.npos) {
                    finished = true;
                    if (!gridFound) {
                        return NULL;
                    }
                }
                if (f_in.eof()) {
                    cerr << endl << "ERROR @ ParserGiD()"
                            << "End of grid label not found" << endl;
                }
            }
        }
    }
    // Throws error message if label was not found.
    if (!gridLabelFound) {
        throw Error("Grid3 label not found.");
    }
    if (gridFound) {
        if (stepsByNumberOfCells) {
            grid = new Grid3(bound, numElems);
        } else {
            grid = new Grid3(bound, steps);
        }
    } else {
        grid = NULL;
    }
    return grid;
}

PlaneWave* ParserGiD::readPlaneWave() {
    string filename;
    string label, value;
    CVecR3 dir, pol;
    GroupElements<Vol> elems;
    Magnitude* mag;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Direction")==0) {
            dir = strToCartesianVector(value);
        } else if (label.compare("Polarization") == 0) {
            pol = strToCartesianVector(value);
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Layer Box") == 0) {
            elems = boundToElemGroup(value);
        } else if (label.compare("Number of elements")==0) {
            UInt nE = atoi(value.c_str());
            elems.clear();
            elems.reserve(nE);
            for (UInt i = 0; i < nE; i++) {
                ElementId id;
                f_in >> id;
                elems.add(mesh_->elems().getId(id));
            }
        } else if (label.compare("End of Planewave")==0) {
            return new PlaneWave(mag, elems, dir, pol);
        }
    }
    throw Error("End of Planewave label not found.");
}

Dipole* ParserGiD::readDipole() {
    //    GroupElements<Vol> elems;
    //    Real length = 0.0;
    //    CVecR3 orientation;
    //    CVecR3 position;
    //    MagnitudeGaussian* mag = NULL;
    //
    //    string line;
    //    bool finished = false;
    //    char* pEnd;
    //    while(!finished && !f_in.eof()) {
    //        getline(f_in, line);
    //        if (line.find("End of puntual excitation") == line.npos) {
    //            ElementId id = ElementId(strtol(line.c_str(), &pEnd, 10));
    //            //            Volume<>* elem = mesh_->elems().get(id);
    //            //            elems.add(elem);
    //        } else
    //            finished = true;
    //    }
    //    if (!finished) {
    //        cerr << endl << "ERROR @ ParserGiD::readDipoleEMSource: "
    //                << "End of excitation type label not found. "
    //                << endl;
    //    }
    //    //
    //    return new Dipole(mag, elems, length, orientation, position);
}

PortWaveguide* ParserGiD::readPortWaveguide() {
    UInt numElements = 0;
    Magnitude* mag;
    WaveportShape shape = WaveportShape::rectangular;
    PortWaveguide::ExcitationMode excitationMode = PortWaveguide::TE;
    pair<UInt,UInt> mode(1,0);
    string line, label, value;
    GroupElements<const Surf> surfs;
    bool finished = false;
    while (!finished && !f_in.eof()) {
        getNextLabelAndValue(label,value);
        if (!label.compare("Shape")) {
            if (value.find("Rectangular") != value.npos) {
                shape = WaveportShape::rectangular;
            } else {
                throw Error("Unreckognized waveguide port shape.");
            }
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("ExcitationMode") == 0) {
            if (value.find("TE") != value.npos) {
                excitationMode = PortWaveguide::TE;
            } else if (value.find("TM") != value.npos) {
                excitationMode = PortWaveguide::TM;
            }
        } else if (label.compare("FirstMode") == 0) {
            mode.first = atoi(value.c_str());
        } else if (label.compare("SecondMode") == 0) {
            mode.second = atoi(value.c_str());
        } else if (label.compare("Number of elements") == 0) {
            numElements = atoi(value.c_str());
        } else if (label.compare("Elements") == 0) {
            UInt e, f;
            vector<Face> faces;
            for (UInt i = 0; i < numElements; i++) {
                f_in >> e >> f;
                const ElemR* elem = mesh_->elems().getId(ElementId(e));
                if (elem->is<VolR>()) {
                    const VolR* vol = elem->castTo<VolR>();
                    faces.push_back(Face(vol,f-1));
                } else if (elem->is<SurfR>()) {
                    surfs.add(elem->castTo<SurfR>());
                }
            }
            surfs.add(mesh_->getSurfsMatching(faces));
            if (surfs.size() < faces.size()) {
                surfs.printInfo();
                throw Error("Could not find surfaces matching element faces.");
            }
            if (surfs.size() == 0) {
                throw Error("No surfaces read on waveguide port.");
            }
        } else if (label.find("End of Waveguide port") != label.npos) {
            finished = true;
        }
        if (f_in.eof()) {
            throw Error("End of Waveguide port not found");
        }
    }
    // Throws error message if finished was not updated.
    if (!finished) {
        throw Error("End of excitation type label not found.");
    }
    if (shape == WaveportShape::rectangular) {
        return new PortWaveguideRectangular(mag, surfs, excitationMode, mode);
    } else {
        throw Error("Unsupported Waveport shape.");
    }
}

PortTEM* ParserGiD::readPortTEM() {
    UInt numElements = 0;
    Magnitude* mag;
    PortTEM::ExcitationMode excitationMode = PortTEM::voltage;
    CVecR3 origin;
    Real innerRadius, outerRadius;
    string line, label, value;
    GroupElements<const Surf> surfs;
    bool finished = false;
    while (!finished && !f_in.eof()) {
        getNextLabelAndValue(label,value);
        if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (!label.compare("ExcitationMode")) {
            if (value.find("Voltage") != value.npos) {
                excitationMode = PortTEM::voltage;
            } else if (value.find("Current") != value.npos) {
                excitationMode = PortTEM::current;
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
            UInt e, f;
            vector<Face> faces;
            for (UInt i = 0; i < numElements; i++) {
                f_in >> e >> f;
                const ElemR* elem = mesh_->elems().getId(ElementId(e));
                if (elem->is<VolR>()) {
                    const VolR* vol = elem->castTo<VolR>();
                    faces.push_back(Face(vol,f-1));
                } else if (elem->is<SurfR>()) {
                    surfs.add(elem->castTo<SurfR>());
                }
            }
            surfs.add(mesh_->getSurfsMatching(faces));
            if (surfs.size() < faces.size()) {
                surfs.printInfo();
                throw Error("Could not find surfaces matching element faces.");
            }
            if (surfs.size() == 0) {
                throw Error("No surfaces read on TEM port.");
            }
        } else if (label.find("End of TEM port") != label.npos) {
            finished = true;
        }
        if (f_in.eof()) {
            throw Error("End of TEM port not found");
        }
    }
    // Throws error message if finished was not updated.
    if (!finished) {
        throw Error("End of excitation type label not found.");
    }
    return new PortTEMCoaxial(mag, surfs, excitationMode, origin,
            innerRadius, outerRadius);
}

Generator* ParserGiD::readGenerator() {
    GroupElements<Nod> elems;
    Magnitude* mag;
    Generator::Type type;
    Generator::Hardness hardness;
    string filename;
    string label, value;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Type")==0) {
            type = strToGeneratorType(value);
        } else if (label.compare("Hardness")==0) {
            hardness = strToGeneratorHardness(value);
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Number of elements")==0) {
            UInt nE = atoi(value.c_str());
            vector<ElementId> nodes;
            nodes.reserve(nE);
            for (UInt i = 0; i < nE; i++) {
                UInt e;
                f_in >> e;
                CoordinateId id = CoordinateId(e);
                NodR* node = new NodR(*mesh_, ElementId(0), &id);
                mesh_->elems().addId(node);
                nodes.push_back(node->getId());
            }
            elems = mesh_->elems().getId(nodes);
        } else if (label.compare("End of Generator")==0) {
            return new Generator(mag, elems, type, hardness);
        }
    }
    throw("End of Generator label not found.");
}

SourceOnLine* ParserGiD::readSourceOnLine() {
    SourceOnLine::Type type;
    SourceOnLine::Hardness hardness;
    Magnitude* mag;
    vector<ElementId> ids;
    string filename;
    string label, value;
    while(!f_in.eof()) {
        getNextLabelAndValue(label, value);
        if (label.compare("Type")==0) {
            type = strToNodalType(value);
        } else if (label.compare("Hardness")==0) {
            hardness = strToNodalHardness(value);
        } else if (label.compare("Excitation") == 0) {
            mag = readMagnitude(value);
        } else if (label.compare("Number of elements")==0) {
            UInt nE = atoi(value.c_str());
            ids.reserve(ids.size() + nE);
            for (UInt i = 0; i < nE; i++) {
                UInt e;
                f_in >> e;
                ids.push_back(ElementId(e));
            }
        } else if (label.compare("End of Source_on_line")==0) {
            GroupElements<Lin> lines = mesh_->elems().getId(ids);
            return new SourceOnLine(mag, lines, type, hardness);
        }
    }
    throw Error("End of Nodal label not found.");
}

OutRq<void>::Type ParserGiD::strToOutputType(string str) const {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return OutRqBase::electric;
    } else if (str.compare("magneticField")==0) {
        return OutRqBase::magnetic;
    } else if (str.compare("electricFieldNormals")==0) {
        return OutRqBase::electricFieldNormals;
    } else if (str.compare("magneticFieldNormals")==0) {
        return OutRqBase::magneticFieldNormals;
        //	} else if (str.compare("powerDensity")==0) {
        //		return OutputRequest::powerDensity;
        //	} else if (str.compare("power")==0) {
        //		return OutputRequest::power;
    } else if (str.compare("current")==0) {
        return OutRqBase::current;
    } else if (str.compare("voltage")==0) {
        return OutRqBase::voltage;
    } else if (str.compare("bulkCurrentElectric")==0) {
        return OutRqBase::bulkCurrentElectric;
    } else if (str.compare("bulkCurrentMagnetic")==0) {
        return OutRqBase::bulkCurrentMagnetic;
    } else if (str.compare("farField")==0) {
        return OutRqBase::electric;
    } else {
        throw Error("Unrecognized output type: " + str);
    }
}

ParserGiD::SIBCType ParserGiD::strToSIBCType(string str) {
    str = trim(str);
    if (str.compare("File")==0) {
        return sibc;
    } else if (str.compare("Layers")==0) {
        return multilayer;
    } else {
        throw Error("Unrecognized SIBC type: " + str);
    }
}

Generator::Type ParserGiD::strToGeneratorType(string str) {
    str = trim(str);
    if (str.compare("voltage")==0) {
        return Generator::voltage;
    } else if (str.compare("current")==0) {
        return Generator::current;
    } else {
        throw Error("Unrecognized generator type: " + str);
    }
}

Generator::Hardness ParserGiD::strToGeneratorHardness(string str) {
    str = trim(str);
    if (str.compare("soft")==0) {
        return Generator::soft;
    } else if (str.compare("hard")==0) {
        return Generator::hard;
    } else {
        throw Error("Unrecognized generator hardness: " + str);
    }
}

OptionsMesher::BoundType ParserGiD::strToBoundType(string str) {
    str = trim(str);
    if (str.compare("PEC")==0) {
        return OptionsMesher::pec;
    } else if (str.compare("PMC")==0) {
        return OptionsMesher::pmc;
    } else if (str.compare("PML")==0) {
        return OptionsMesher::pml;
    } else if (str.compare("Periodic")==0) {
        return OptionsMesher::periodic;
    } else if (str.compare("MUR1")==0) {
        return OptionsMesher::mur1;
    } else if (str.compare("MUR2")==0) {
        return OptionsMesher::mur2;
    } else {
        throw Error("Unrecognized bound label: " + str);
    }
}

PhysicalModel::Type ParserGiD::strToMaterialType(string str) {
    str = trim(str);
    if (str.compare("PEC")==0) {
        return PhysicalModel::PEC;
    } else if (str.compare("PMC")==0) {
        return PhysicalModel::PMC;
    } else if (str.compare("PML")==0) {
        return PhysicalModel::PML;
    } else if (str.compare("SMA")==0) {
        return PhysicalModel::SMA;
    } else if (str.compare("Classic")==0) {
        return PhysicalModel::classic;
    } else if (str.compare("Dispersive")==0) {
        return PhysicalModel::elecDispersive;
    } else if (str.compare("Anisotropic")==0) {
        return PhysicalModel::anisotropic;
    } else if (str.compare("SIBC")==0) {
        return PhysicalModel::isotropicsibc;
    } else if (str.compare("Wire")==0) {
        return PhysicalModel::wire;
    } else if (str.find("Conn_") != string::npos) {
        return PhysicalModel::multiport;
    } else {
        throw Error("Unrecognized material label: " + str);
    }
}

PMMultiport::Type ParserGiD::strToMultiportType(string str) {
    str = trim(str);
    if (str.compare("Conn_short")==0) {
        return PMMultiport::shortCircuit;
    } else if (str.compare("Conn_open")==0) {
        return PMMultiport::openCircuit;
    } else if (str.compare("Conn_matched")==0) {
        return PMMultiport::matched;
    } else if (str.compare("Conn_sRLC")==0) {
        return PMMultiport::sRLC;
    } else if (str.compare("Conn_pRLC")==0) {
        return PMMultiport::pRLC;
    } else if (str.compare("Conn_sLpRC")==0) {
        return PMMultiport::sLpRC;
    } else {
        throw Error("Unrecognized multiport label: " + str);
    }
}

pair<CVecR3, CVecR3> ParserGiD::strToBound(const string& value) {
    UInt begin = value.find_first_of("{");
    UInt end = value.find_last_of("}");
    string aux = value.substr(begin+1,end-1);
    stringstream iss(aux);
    CVecR3 max, min;
    for (UInt i = 0; i < 3; i++) {
        iss >> max(i);
    }
    for (UInt i = 0; i < 3; i++) {
        iss >> min(i);
    }
    pair<CVecR3,CVecR3> bound(min, max);
    return bound;
}


CVecR3 ParserGiD::strToCVecR3(const string& str) {
    stringstream ss(str);
    CVecR3 res;
    ss >> res(x) >> res(y) >> res(z);
    return res;
}

SourceOnLine::Type ParserGiD::strToNodalType(string str) {
    str = trim(str);
    if (str.compare("electricField")==0) {
        return SourceOnLine::electric;
    } else if (str.compare("magneticField")==0) {
        return SourceOnLine::magnetic;
    } else {
        throw Error("Unrecognized nodal type: " + str);
    }
}

SourceOnLine::Hardness ParserGiD::strToNodalHardness(string str) {
    str = trim(str);
    if (str.compare("soft")==0) {
        return SourceOnLine::soft;
    } else if (str.compare("hard")==0) {
        return SourceOnLine::hard;
    } else {
        throw Error("Unrecognized nodal hardness: " + str);
        return SourceOnLine::soft;
    }
}

string ParserGiD::readVersion() {
    string line, label, value;
    bool formatFound = false;
    bool versionFound = false;
    string format, version;
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
        cerr << endl << "ERROR @ ParserGiD: "
                << "EoF was reached but format label was not found." << endl;
    }
    if (!versionFound) {
        cerr << endl << "ERROR @ ParserGiD: "
                << "EoF was reached but version label was not found." << endl;
    }
    return version;
}

ParserGiD::GiDOutputType ParserGiD::strToGidOutputType(string str) {
    str = trim(str);
    if (str.compare("OutRq_on_point")==0) {
        return ParserGiD::outRqOnPoint;
    } else if (str.compare("OutRq_on_line")==0) {
        return ParserGiD::outRqOnLine;
    } else if (str.compare("OutRq_on_surface")==0) {
        return ParserGiD::outRqOnSurface;
    } else if (str.compare("OutRq_on_volume")==0) {
        return ParserGiD::outRqOnVolume;
    } else if (str.compare("Bulk_current_on_surface")==0) {
        return ParserGiD::bulkCurrentOnSurface;
    } else if (str.compare("Bulk_current_on_volume")==0) {
        return ParserGiD::bulkCurrentOnVolume;
    } else if (str.compare("farField")==0) {
        return ParserGiD::farField;
    } else {
        cerr << endl << "ERROR @ Parser: Unrecognized label." << endl;
        return ParserGiD::outRqOnPoint;
    }
}

Domain ParserGiD::strToDomain(string line) {
    UInt timeDomain;
    Real initialTime;
    Real finalTime;
    Real samplingPeriod;
    UInt frequencyDomain;
    Real initialFrequency;
    Real finalFrequency;
    Real frequencyStep;
    UInt logFrequencySweep;
    UInt usingTransferFunction;
    string transferFunctionFile;
    stringstream ss(line);
    ss >> timeDomain >> initialTime >> finalTime >> samplingPeriod
    >> frequencyDomain >> initialFrequency >> finalFrequency >>frequencyStep
    >> logFrequencySweep >> usingTransferFunction >> transferFunctionFile;
    //
    transferFunctionFile.erase(std::remove( transferFunctionFile.begin(),
            transferFunctionFile.end(), '\"' ),transferFunctionFile.end());
    //
    return (Domain(toBool(timeDomain), initialTime, finalTime, samplingPeriod,
            toBool(frequencyDomain), initialFrequency, finalFrequency,
            frequencyStep,	toBool(logFrequencySweep),
            toBool(usingTransferFunction), transferFunctionFile));
}

Magnitude* ParserGiD::readMagnitude(const string typeIn) {
    string type = typeIn;
    type = trim(type);
    bool finished = false;
    string label, value;
    if (type.compare("Gaussian") == 0) {
        Real delay, spread;
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
                return new Magnitude(new Gaussian(spread, delay));
            }
        }
    } else if (type.compare("File") == 0) {
        string excName;
        while (!finished && !f_in.eof()) {
            getNextLabelAndValue(label, value);
            if (label.compare("Filename") == 0) {
                excName = trim(value);
                finished = true;
            }
            if (finished) {
                return new MagnitudeNumerical(getFolder() + excName);
            }
        }
    }
    throw Error("Unable to recognize magnitude type when reading excitation.");
}

OptionsMesher::Mesher ParserGiD::strToMesher(string str) {
    str = trim(str);
    if (str.compare("ugrMesher")==0) {
        return OptionsMesher::ugrMesher;
    } else if (str.compare("zMesher")==0) {
        return OptionsMesher::zMesher;
    } else if (str.compare("OpenFOAM")==0) {
        return OptionsMesher::openfoam;
    } else if (str.compare("None")==0) {
        return OptionsMesher::none;
    } else {
        throw Error("Unreckognized label: " + str);
    }
}

OptionsMesher::Mode ParserGiD::strToMesherMode(string str)  {
    str = trim(str);
    if (str.compare("Structured")==0) {
        return OptionsMesher::structured;
    } else if (str.compare("Relaxed")==0) {
        return OptionsMesher::relaxed;
    } else if (str.compare("Slanted")==0) {
        return OptionsMesher::slanted;
    } else if (str.compare("Conformal")==0) {
        return OptionsMesher::conformal;
    } else {
        throw Error("Unreckognized label: " + str);
    }
}

OptionsSolver::Solver ParserGiD::strToSolver(string str) {
    str = trim(str);
    if (str.compare("ugrfdtd")==0) {
        return OptionsSolver::Solver::ugrfdtd;
    } else if (str.compare("cudg3d")==0) {
        return OptionsSolver::Solver::cudg3d;
    } else if (str.compare("none")==0) {
        return OptionsSolver::Solver::none;
    } else {
        throw Error("Unreckognized label: " + str);
    }
}

OptionsSolverFDTD::CompositeModel ParserGiD::strToCompositeModel(string str) {
    str = trim(str);
    if (str.compare("None")==0) {
        return OptionsSolverFDTD::CompositeModel::none;
    } else if (str.compare("DigFilt")==0) {
        return OptionsSolverFDTD::CompositeModel::digFilt;
    } else if (str.compare("MIBC")==0) {
        return OptionsSolverFDTD::CompositeModel::mibc;
    } else if (str.compare("ADE")==0) {
        return OptionsSolverFDTD::CompositeModel::ade;
    } else {
        throw Error("Unreckognized label: " + str);
    }
}

OptionsSolverFDTD::WireModel ParserGiD::strToWireModel(string str) {
    str = trim(str);
    if (str.compare("Old")==0) {
        return OptionsSolverFDTD::WireModel::oldWireModel;
    } else if (str.compare("Transition")==0) {
        return OptionsSolverFDTD::WireModel::transitionWireModel;
    } else if (str.compare("New")==0) {
        return OptionsSolverFDTD::WireModel::newWireModel;
    } else {
        throw Error("Unrecognized label: " + str);
    }
}

OptionsSolverFDTD::InductanceModel ParserGiD::strToInductanceModel(string str) {
    str = trim(str);
    if (str.compare("Boutayeb")==0) {
        return OptionsSolverFDTD::InductanceModel::boutayeb;
    } else if (str.compare("Ledfelt")==0) {
        return OptionsSolverFDTD::InductanceModel::ledfelt;
    } else if (str.compare("Berenger")==0) {
        return OptionsSolverFDTD::InductanceModel::berenger;
    } else {
        throw Error("Unrecognized Inductance model label: " + str);
    }
}

bool ParserGiD::checkVersionCompatibility(const string version) const {
    bool versionMatches =
            atof(version.c_str()) == atof(string(APP_VERSION).c_str());
    if (!versionMatches) {
        throw Error("File version " + version + " is not supported.");
    }
    return versionMatches;
}

OptionsSolverFDTD::PMLBacking ParserGiD::strToPMLBacking(const string& inStr) {
    string str = inStr;
    str = trim(str);
    if (str.compare("None")==0) {
        return OptionsSolverFDTD::PMLBacking::none;
    } else if (str.compare("Mur1")==0) {
        return OptionsSolverFDTD::PMLBacking::mur1;
    } else if (str.compare("Mur2")==0) {
        return OptionsSolverFDTD::PMLBacking::mur2;
    } else {
        throw Error("Unrecognized PML Backing label: " + str);
        return OptionsSolver::PMLBacking::none;
    }
}

OptionsSolverFDTD::NoNF2FF ParserGiD::strToNoNF2FF(const string& inStr) {
    string str = inStr;
    str = trim(str);
    if (str.compare("None")==0) {
        return OptionsSolverFDTD::NoNF2FF::none;
    } else if (str.compare("back")==0) {
        return OptionsSolverFDTD::NoNF2FF::back;
    } else if (str.compare("front")==0) {
        return OptionsSolverFDTD::NoNF2FF::front;
    } else if (str.compare("left")==0) {
        return OptionsSolverFDTD::NoNF2FF::left;
    } else if (str.compare("right")==0) {
        return OptionsSolverFDTD::NoNF2FF::right;
    } else if (str.compare("down")==0) {
        return OptionsSolverFDTD::NoNF2FF::down;
    } else if (str.compare("up")==0) {
        return OptionsSolverFDTD::NoNF2FF::up;
    } else {
        throw Error("Unrecognized No NF2FF label: " + str);
        return OptionsSolverFDTD::NoNF2FF::none;
    }
}

PoleResidue ParserGiD::readPoleResiduePair(ifstream& stream) {
    std::string line;
    getline(stream, line);
    std::size_t prev = 0, pos;
    vector<string> wordVector;
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
    PoleResidue res;
    res.first = complex<Real>(atof(wordVector[0].c_str()),
            atof(wordVector[1].c_str()));
    res.second = complex<Real>(atof(wordVector[2].c_str()),
            atof(wordVector[3].c_str()));
    return res;
}

PMVolumeAnisotropic::Model ParserGiD::strToAnisotropicModel(string label) {
    string str = label;
    str = trim(str);
    if (str.compare("Crystal")==0) {
        return PMVolumeAnisotropic::Model::crystal;
    } else if (str.compare("Ferrite")==0) {
        return PMVolumeAnisotropic::Model::ferrite;
    } else {
        throw Error("Unrecognized Anisotropic Model: " + str);
    }
}

LocalAxes ParserGiD::strToLocalAxes(const string& str) {
    UInt begin = str.find_first_of("{");
    UInt end = str.find_first_of("}");
    CVecR3 eulerAngles = strToCVecR3(str.substr(begin+1,end-1));
    begin = str.find_last_of("{");
    end = str.find_last_of("}");
    CVecR3 origin = strToCVecR3(str.substr(begin+1,end-1));
    return LocalAxes(eulerAngles, origin);
}

