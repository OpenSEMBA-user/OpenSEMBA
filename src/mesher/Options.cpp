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
#include "Options.h"

#include "physicalModel/bound/PEC.h"
#include "physicalModel/bound/PMC.h"
#include "physicalModel/bound/SMA.h"
#include "physicalModel/bound/PML.h"
#include "physicalModel/bound/Periodic.h"
#include "physicalModel/bound/Mur1.h"
#include "physicalModel/bound/Mur2.h"

namespace SEMBA {
namespace Mesher {

Options::Options() {
    locationInMeshSet_ = false;
    mesher_ = Mesher::zMesher;
    mode_ = Mode::conformal;
    bruteForceVolumes_ = false;
    scalingFactor_ = 1.0;
    postmshExport_ = true;
    vtkExport_ = false;
    slanted_ = false;
    slantedCoordCriterion_ = SlantedCoordCriterion::structured;
    slantedRelaxedLength_ = 0.0;
    slantedGenerateIntermedial_ = false;
    slantedThreshold_ = 0.0;
    gridStep_ = Math::CVecR3(0.0);
    forbiddenLength_ = (Math::Real) (1.0 / 3.0);
}

void Options::printInfo() const {
    std::cout << " --- Meshing parameters info --- " << std::endl;
    if (mesher_ == Mesher::openfoam) {
        std::cout << " Using openfoam mesher." << std::endl;
        if (locationInMeshSet_) {
            std::cout << " Location in mesh:   "
                << locationInMesh_ << std::endl;
        }
    }
    if (isStructured()) {
        std::cout << " Generating structured mesh." << std::endl;
    }
    std::cout << "Lower X Bound: "
        << toStr(getBoundTermination(0, 0)) << std::endl;
    std::cout << "Upper X Bound: "
        << toStr(getBoundTermination(0, 1)) << std::endl;
    std::cout << "Lower Y Bound: "
        << toStr(getBoundTermination(1, 0)) << std::endl;
    std::cout << "Upper Y Bound: "
        << toStr(getBoundTermination(1, 1)) << std::endl;
    std::cout << "Lower Z Bound: "
        << toStr(getBoundTermination(2, 0)) << std::endl;
    std::cout << "Upper Z Bound: "
        << toStr(getBoundTermination(2, 1)) << std::endl;
    std::cout << " --- End of Meshing parameters info ---" << std::endl;
}

void Options::printHelp() const {}

std::string Options::toStr(const PhysicalModel::Bound::Bound* val) {
    if (val == NULL) {
        return "Undefined";
    }
    if (dynamic_cast<const PhysicalModel::Bound::PEC*>(val) != NULL) {
        return "PEC";
    }
    if (dynamic_cast<const PhysicalModel::Bound::PMC*>(val) != NULL) {
        return "PMC";
    }
    if (dynamic_cast<const PhysicalModel::Bound::PML*>(val) != NULL) {
        return "PML";
    }
    if (dynamic_cast<const PhysicalModel::Bound::Periodic*>(val) != NULL) {
        return "Periodic";
    }
    if (dynamic_cast<const PhysicalModel::Bound::Mur1*>(val) != NULL) {
        return "MUR1";
    }
    if (dynamic_cast<const PhysicalModel::Bound::Mur2*>(val) != NULL) {
        return "MUR2";
    }
    return "Undefined";

}

const Math::CVecR3& Options::getLocationInMesh() const {
    return locationInMesh_;
}

bool Options::isLocationInMeshSet() const {
    return locationInMeshSet_;
}

bool Options::isBruteForceVolumes() const {
    return bruteForceVolumes_;
}

bool Options::isStructured() const {
    return (mode_ == Mode::structured);
}

bool Options::isRelaxed() const {
    if (mode_ == Mode::slanted || mode_ == Mode::relaxed) {
        return true;
    }
    return false;
}

std::string Options::getScaleFactor() const {
    return scaleFactorValue_;
}

Math::Real Options::getScalingFactor() const {
    return scalingFactor_;
}

Options::Mesher Options::getMesher() const {
    return mesher_;
}

const PhysicalModel::Bound::Bound* Options::getBoundTermination(
    const std::size_t d,
    const std::size_t p) const {
    assert(d < 3);
    assert(p < 2);
    return dynamic_cast<const PhysicalModel::Bound::Bound*>(boundTermination_(d, p));
}

const Geometry::BoundTerminations3& Options::getBoundTerminations() const {
    return boundTermination_;
}

void Options::setBoundTermination(
    const std::size_t d,
    const std::size_t p,
    const PhysicalModel::Bound::Bound* bound) {
    assert(d < 3);
    assert(p < 2);
    boundTermination_.setBound(d, p, bound);
}
void Options::addArguments(Argument::Group& args) const {
    args.addOption(new Argument::Option<std::string>("Mesher", "mesher"))
        .choices({{"openfoam"}, {"zMesher"}, {"confMesher"}});
    args.addOption(new Argument::Switch("Brute force volumes",
        "bruteForceVolumes"));
    args.addOption(new Argument::Switch("VTK Export", "vtkexport"));
    args.addOption(
        new Argument::Option<Math::Real, Math::Real, Math::Real>("gridstep"));
    args.addOption(new Argument::Option<std::string>("Mode", "mode"))
        .choices({{"Structured"}, {"Relaxed"}, {"Slanted"}, {"Conformal"}});
    args.addOption(
        new Argument::Option<Math::Real>("Forbidden length", "lenght"));
}

void Options::set(const Solver::Settings& opts) {
    if (opts.existsName("Mesher")) {
        setMesher(strToMesher(opts("Mesher").getString()));
    }
    if (opts.existsName("Brute force volumes")) {
        setBruteForceVolumes(opts("Brute force volumes").getBool());
    }
    if (opts.existsName("VTK Export")) {
        setVtkExport(opts("VTK Export").getBool());
    }
    if (opts.existsName("postmsh Export")) {
        setPostmshExport(opts("postmsh Export").getBool());
    }
    if (opts.existsName("Slanted wires")) {
        setSlanted(opts("Slanted wires").getBool());
    }
    if (opts.existsName("Slanted coordinate criterion")) {
        setSlantedCoordCriterion(
            strToCoordCriterion(
                opts("Slanted coordinate criterion").getString()));
    }
    if (opts.existsName("Slanted relaxed length")) {
        setSlantedRelaxedLength(
            opts("Slanted relaxed length").get<Math::Real>());
    }
    if (opts.existsName("Slanted generate intermedial coords")) {
        setSlantedGenerateIntermedialCoords(
            opts("Slanted generate intermedial coords").getBool());
    }
    if (opts.existsName("Slanted threshold")) {
        setSlantedThreshold(opts("Slanted threshold").get<Math::Real>());
    }
    if (opts.existsName("Mode")) {
        setMode(strToMesherMode(opts("Mode").getString()));
    }
    if (opts.existsName("Forbidden length")) {
        setForbiddenLength(opts("Forbidden length").getReal());
    }
    if (opts.existsName("Scale factor value")) {
        setScaleFactorValue(opts("Scale factor value").getString());
    }
    if (opts.existsName("Location in mesh")) {
        setLocationInMesh(strToCVecR3(opts("Location in mesh").getString()));
    }
    if (opts.existsName("Geometry scaling factor")) {
        setScalingFactor(opts("Geometry scaling factor").getReal());
    }
    if (opts.existsName("Upper x bound")) {
        setBoundTermination(Math::Constants::x, Math::Constants::U,
                            strToBoundType(opts("Upper x bound").getString()));
    }
    if (opts.existsName("Lower x bound")) {
        setBoundTermination(Math::Constants::x, Math::Constants::L,
                            strToBoundType(opts("Lower x bound").getString()));
    }
    if (opts.existsName("Upper y bound")) {
        setBoundTermination(Math::Constants::y, Math::Constants::U,
                            strToBoundType(opts("Upper y bound").getString()));
    }
    if (opts.existsName("Lower y bound")) {
        setBoundTermination(Math::Constants::y, Math::Constants::L,
                            strToBoundType(opts("Lower y bound").getString()));
    }
    if (opts.existsName("Upper z bound")) {
        setBoundTermination(Math::Constants::z, Math::Constants::U,
                            strToBoundType(opts("Upper z bound").getString()));
    }
    if (opts.existsName("Lower z bound")) {
        setBoundTermination(Math::Constants::z, Math::Constants::L,
                            strToBoundType(opts("Lower z bound").getString()));
    }
}

void Options::setBruteForceVolumes(bool bruteForceVolumes) {
    bruteForceVolumes_ = bruteForceVolumes;
}

void Options::setLocationInMesh(const Math::CVecR3& locationInMesh) {
    locationInMeshSet_ = true;
    locationInMesh_ = locationInMesh;
}

void Options::setLocationInMeshSet(bool locationInMeshSet) {
    locationInMeshSet_ = locationInMeshSet;
}

void Options::setMesher(Mesher mesher) {
    mesher_ = mesher;
}

void Options::setMode(Mode mode) {
    mode_ = mode;
}

void Options::setScaleFactorValue(const std::string& scaleFactorValue) {
    scaleFactorValue_ = scaleFactorValue;
}

void Options::setScalingFactor(const Math::Real& scalingFactor) {
    scalingFactor_ = scalingFactor;
}

void Options::setForbiddenLength(const Math::Real& edgeFraction) {}

Math::Real Options::getForbiddenLength() const {
    if (mode_ == Mode::slanted) {
        return -1.0;
    } else {
        return forbiddenLength_;
    }
}

const std::string& Options::getOutputName() const {
    return outputName_;
}

void Options::setOutputName(const std::string& outputName) {
    outputName_ = outputName;
}

const Math::CVecR3& Options::getGridStep() const {
    return gridStep_;
}

void Options::setGridStep(const Math::CVecR3& gridStep) {
    gridStep_ = gridStep;
}

bool Options::isVtkExport() const {
    return vtkExport_;
}

bool Options::isSlanted() const {
    return slanted_;
}

Options::SlantedCoordCriterion Options::getSlantedCoordCriterion() const {
    return slantedCoordCriterion_;
}

Math::Real Options::getSlantedRelaxedLength() const {
    return slantedRelaxedLength_;
}

bool Options::isSlantedGenerateIntermedialCoords() const {
    return slantedGenerateIntermedial_;
}

bool Options::isSlantedThreshold() const {
    if (Math::Util::lowerEqual(slantedThreshold_, 0.0)) {
        return false;
    }
    return true;
}

Math::Real Options::getSlantedThreshold() const {
    return slantedThreshold_;
}

bool Options::isPostmshExport() const {
    return postmshExport_;
}

void Options::setPostmshExport(bool postmshExport) {
    postmshExport_ = postmshExport;
}

void Options::setVtkExport(bool vtkExport) {
    vtkExport_ = vtkExport;
}

void Options::setSlanted(const bool& slanted) {
    slanted_ = slanted;
}

void Options::setSlantedCoordCriterion(
        const Options::SlantedCoordCriterion& slantedCoordCriterion) {
    slantedCoordCriterion_ = slantedCoordCriterion;
}

void Options::setSlantedRelaxedLength(const Math::Real& slantedRelaxedLength) {
    slantedRelaxedLength_ = slantedRelaxedLength;
}

void Options::setSlantedGenerateIntermedialCoords(const bool& genInt) {
    slantedGenerateIntermedial_ = genInt;
}

void Options::setSlantedThreshold(const Math::Real& slantedThreshold) {
    slantedThreshold_ = slantedThreshold;
}


bool Options::isGridStepSet() const {
    return (gridStep_ != Math::CVecR3(0.0));
}

Options::Mesher Options::strToMesher(std::string str) {
    if (str.compare("ConformalMesher")==0) {
        return Mesher::conformalMesher;
    } else if (str.compare("zMesher")==0) {
        return Mesher::zMesher;
    } else if (str.compare("OpenFOAM")==0) {
        return Mesher::openfoam;
    } else if (str.compare("None")==0) {
        return Mesher::none;
    } else {
        throw std::logic_error("Unreckognized label: " + str);
    }
}

Options::Mode Options::strToMesherMode(std::string str) {
    if (str.compare("Structured")==0) {
        return Mode::structured;
    } else if (str.compare("Relaxed")==0) {
        return Mode::relaxed;
    } else if (str.compare("Slanted")==0) {
        return Mode::slanted;
    } else if (str.compare("Conformal")==0) {
        return Mode::conformal;
    } else {
        throw std::logic_error("Unreckognized label: " + str);
    }
}

Math::CVecR3 Options::strToCVecR3(const std::string& str) {
    std::stringstream ss(str);
    Math::CVecR3 res;
    ss >> res(Math::Constants::x)
        >> res(Math::Constants::y)
        >> res(Math::Constants::z);
    return res;
}

std::pair<Math::CVecR3, Math::CVecR3> Options::strToBox(
        const std::string& value) {
    std::size_t begin = value.find_first_of("{");
    std::size_t end = value.find_last_of("}");
    std::string aux = value.substr(begin + 1, end - 1);
    std::stringstream iss(aux);
    Math::CVecR3 max, min;
    for (std::size_t i = 0; i < 3; i++) {
        iss >> max(i);
    }
    for (std::size_t i = 0; i < 3; i++) {
        iss >> min(i);
    }
    std::pair<Math::CVecR3, Math::CVecR3> bound(min, max);
    return bound;
}

const PhysicalModel::Bound::Bound* Options::strToBoundType(std::string str) {
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

Options::SlantedCoordCriterion Options::strToCoordCriterion(
        const std::string& str) {
    if (str.compare("Structured") == 0) {
        return SlantedCoordCriterion::structured;
    } else if (str.compare("RelaxedPlane") == 0) {
        return SlantedCoordCriterion::relaxedPlane;
    } else if (str.compare("Relaxed") == 0) {
        return SlantedCoordCriterion::relaxed;
    } else if (str.compare("Raw") == 0) {
        return SlantedCoordCriterion::raw;
    } else {
        throw std::logic_error("Unrecognized Slanted Coord Criterion: " + str);
    }
}

}
}
