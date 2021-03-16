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
    mode_ = Mode::conformal;
    scalingFactor_ = 1.0;
    postmshExport_ = true;
    vtkExport_ = false;
    contourRefinement_ = false;
    unwantedConnectionsInfo_ = false;
    slanted_ = false;
    slantedThreshold_ = 1.0;
    gridStep_ = Math::CVecR3(0.0);
    forbiddenLength_ = (Math::Real) (1.0 / 3.0);
}

void Options::printInfo() const {
    std::cout << " --- Meshing parameters info --- " << std::endl;
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
    if (val == nullptr) {
        return "Undefined";
    }
    if (dynamic_cast<const PhysicalModel::Bound::PEC*>(val) != nullptr) {
        return "PEC";
    }
    if (dynamic_cast<const PhysicalModel::Bound::PMC*>(val) != nullptr) {
        return "PMC";
    }
    if (dynamic_cast<const PhysicalModel::Bound::PML*>(val) != nullptr) {
        return "PML";
    }
    if (dynamic_cast<const PhysicalModel::Bound::Periodic*>(val) != nullptr) {
        return "Periodic";
    }
    if (dynamic_cast<const PhysicalModel::Bound::Mur1*>(val) != nullptr) {
        return "MUR1";
    }
    if (dynamic_cast<const PhysicalModel::Bound::Mur2*>(val) != nullptr) {
        return "MUR2";
    }
    return "Undefined";

}

Options::Mode Options::getMode() const {
    return mode_;
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

Math::Real Options::getScalingFactor() const {
    return scalingFactor_;
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
    args.addOption(new Argument::Switch("VTK Export", "vtkexport"));
    args.addOption(
        new Argument::Option<Math::Real, Math::Real, Math::Real>("gridstep"));
    args.addOption(new Argument::Option<std::string>("Mode", "mode"))
        .choices({{"Structured"}, {"Relaxed"}, {"Slanted"}, {"Conformal"}});
    args.addOption(
        new Argument::Option<Math::Real>("Forbidden length", "length"));
}

void Options::set(const Solver::Settings& opts) {
    if (opts.existsName("contourRefinement")) {
        setContourRefinement(opts("contourRefinement").getBool());
    }
    if (opts.existsName("unwantedConnectionsInfo")) {
        setUnwantedConnectionsInfo(opts("unwantedConnectionsInfo").getBool());
    }
    if (opts.existsName(  "vtkExport")) {
        setVtkExport(opts("vtkExport").getBool());
    }
    if (opts.existsName(      "postmshExport")) {
        setPostmshExport(opts("postmshExport").getBool());
    }
    if (opts.existsName("slantedWires")) {
        setSlanted(opts("slantedWires").getBool());
    }
    if (opts.existsName(         "slantedThreshold")) {
        setSlantedThreshold(opts("slantedThreshold").get<Math::Real>());
    }
    if (opts.existsName(             "mode")) {
        setMode(strToMesherMode(opts("mode").getString()));
    }
    if (opts.existsName(        "forbiddenLength")) {
        setForbiddenLength(opts("forbiddenLength").getReal());
    }
    if (opts.existsName(      "geometryScalingFactor")) {
        setScalingFactor(opts("geometryScalingFactor").getReal());
    }
    if (opts.existsName(                        "upperXBound")) {
        setBoundTermination(Math::Constants::x, Math::Constants::U,
                            strToBoundType(opts("upperXBound").getString()));
    }
    if (opts.existsName(                        "lowerXBound")) {
        setBoundTermination(Math::Constants::x, Math::Constants::L,
                            strToBoundType(opts("lowerXBound").getString()));
    }
    if (opts.existsName(                        "upperYBound")) {
        setBoundTermination(Math::Constants::y, Math::Constants::U,
                            strToBoundType(opts("upperYBound").getString()));
    }
    if (opts.existsName(                        "lowerYBound")) {
        setBoundTermination(Math::Constants::y, Math::Constants::L,
                            strToBoundType(opts("lowerYBound").getString()));
    }
    if (opts.existsName(                        "upperZBound")) {
        setBoundTermination(Math::Constants::z, Math::Constants::U,
                            strToBoundType(opts("upperZBound").getString()));
    }
    if (opts.existsName(                        "lowerZBound")) {
        setBoundTermination(Math::Constants::z, Math::Constants::L,
                            strToBoundType(opts("lowerZBound").getString()));
    }
}

void Options::setMode(Mode mode) {
    mode_ = mode;
}

void Options::setScalingFactor(const Math::Real& scalingFactor) {
    scalingFactor_ = scalingFactor;
}

void Options::setForbiddenLength(const Math::Real& forbiddenLength) {
    forbiddenLength_ = forbiddenLength;
}

Math::Real Options::getForbiddenLength() const {
    return forbiddenLength_;
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

bool Options::isContourRefinement() const {
    return contourRefinement_;
}

bool Options::isUnwantedConnectionsInfo() const {
    return unwantedConnectionsInfo_;
}

bool Options::isVtkExport() const {
    return vtkExport_;
}

bool Options::isSlanted() const {
    return slanted_;
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

void Options::setContourRefinement(bool contourRefinement) {
    contourRefinement_ = contourRefinement;
}

void Options::setUnwantedConnectionsInfo(bool rhs) {
    unwantedConnectionsInfo_ = rhs;
}

void Options::setVtkExport(bool vtkExport) {
    vtkExport_ = vtkExport;
}

void Options::setSlanted(const bool& slanted) {
    slanted_ = slanted;
}

void Options::setSlantedThreshold(const Math::Real& slantedThreshold) {
    slantedThreshold_ = slantedThreshold;
}


bool Options::isGridStepSet() const {
    return (gridStep_ != Math::CVecR3(0.0));
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
    std::stringstream sts(str);
    Math::CVecR3 res;
    sts >> res(Math::Constants::x)
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
    return {min, max};
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

}
}
