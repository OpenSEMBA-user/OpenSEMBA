
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
    mesher_ = Mesher::DMesher;
    mode_ = Mode::structured;
    hMesherMode_ = HMesherMode::adapted;
    forbiddenLength_ = 1.0;
    edgePoints_ = 0;
    scalingFactor_ = 1.0;
    hwMeshExport_ = false;
    nfdeExport_ = true;
    postmshExport_ = true;
    postsmbExport_ = false;
    vtkExport_ = false;
    contourRefinement_ = false;
    unwantedConnectionsInfo_ = false;
    structuredCellsInfo_ = false;
    slanted_ = false;
    slantedThreshold_ = 1.0;
    gridStep_ = Math::CVecR3(0.0);

    for (std::size_t d = 0; d < 3; d++) {
        for (std::size_t i = 0; i < 2; i++) {
            boundTermination_.setBound(d, i, new PhysicalModel::Bound::PEC(PhysicalModel::Id(0)));
        }
    }
}

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

Options::Mesher Options::getMesher() const {
    return mesher_;
}

Options::HMesherMode Options::getHMesherMode() const {
    return hMesherMode_;
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

Math::Int Options::getEdgePoints() const {
    return edgePoints_;
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

void Options::setMesher(const Mesher mesher) {
    mesher_ = mesher;
}

void Options::setBoundTermination(
    const std::size_t d,
    const std::size_t p,
    const PhysicalModel::Bound::Bound* bound) {
    assert(d < 3);
    assert(p < 2);
    boundTermination_.setBound(d, p, bound);
}

void Options::setEdgePoints(const Math::Int& sg) {
    edgePoints_ = sg;
}

void Options::set(const json& opts) {
    
    auto exists = [](const json & j, std::string key) 
    {
        return j.find(key) != j.end();
    };

    if (exists(opts, "contourRefinement")) {
        setContourRefinement(opts.at("contourRefinement").get<bool>());
    }
    if (exists(opts, "hMesherMode")) {
        setHMesherMode(strToHMesherMode(opts.at("hMesherMode").get<std::string>()));
    }
    if (exists(opts, "unwantedConnectionsInfo")) {
        setUnwantedConnectionsInfo(opts.at("unwantedConnectionsInfo").get<bool>());
    }
    if (exists(opts, "structuredCellsInfo")) {
        setStructuredCellsInfo(opts.at("structuredCellsInfo").get<bool>());
    }
    if (exists(opts, "nfdeExport")) {
        setHWMeshExport(opts.at("nfdeExport").get<bool>());
    }
    if (exists(opts, "hwMeshExport")) {
        setHWMeshExport(opts.at("hwMeshExport").get<bool>());
    }
    if (exists(opts, "postmshExport")) {
        setPostmshExport(opts.at("postmshExport").get<bool>());
    }
    if (exists(opts, "postsmbExport")) {
        setPostsmbExport(opts.at("postsmbExport").get<bool>());
    }
    if (exists(opts, "vtkExport")) {
        setVtkExport(opts.at("vtkExport").get<bool>());
    }
    if (exists(opts, "slantedWires")) {
        setSlanted(opts.at("slantedWires").get<bool>());
    }
    if (exists(opts, "slantedThreshold")) {
        setSlantedThreshold(opts.at("slantedThreshold").get<double>());
    }
    if (exists(opts, "mesher")) {
        setMesher(strToMesher(opts.at("mesher").get<std::string>()));
    }
    if (exists(opts, "mode")) {
        setMode(strToMesherMode(opts.at("mode").get<std::string>()));
    }
    if (exists(opts, "edgePoints")) {
        setEdgePoints(opts.at("edgePoints").get<int>());
    }
    if (exists(opts, "forbiddenLength")) {
        setForbiddenLength(opts.at("forbiddenLength").get<double>());
    }
    if (exists(opts, "geometryScalingFactor")) {
        setScalingFactor(opts.at("geometryScalingFactor").get<double>());
    }
    if (exists(opts, "upperXBound")) {
        setBoundTermination(Math::Constants::x, Math::Constants::U,
                            strToBoundType(opts.at("upperXBound").get<std::string>()));
    }
    if (exists(opts, "lowerXBound")) {
        setBoundTermination(Math::Constants::x, Math::Constants::L,
                            strToBoundType(opts.at("lowerXBound").get<std::string>()));
    }
    if (exists(opts, "upperYBound")) {
        setBoundTermination(Math::Constants::y, Math::Constants::U,
                            strToBoundType(opts.at("upperYBound").get<std::string>()));
    }
    if (exists(opts, "lowerYBound")) {
        setBoundTermination(Math::Constants::y, Math::Constants::L,
                            strToBoundType(opts.at("lowerYBound").get<std::string>()));
    }
    if (exists(opts, "upperZBound")) {
        setBoundTermination(Math::Constants::z, Math::Constants::U,
                            strToBoundType(opts.at("upperZBound").get<std::string>()));
    }
    if (exists(opts, "lowerZBound")) {
        setBoundTermination(Math::Constants::z, Math::Constants::L,
                            strToBoundType(opts.at("lowerZBound").get<std::string>()));
    }
}

void Options::setMode(const Mode mode) {
    mode_ = mode;
}

void Options::setHMesherMode(const HMesherMode rhs) {
    hMesherMode_ = rhs;
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

bool Options::isStructuredCellsInfo() const {
    return structuredCellsInfo_;
}

bool Options::isNfdeExport() const {
    return nfdeExport_;
}

bool Options::isHWMeshExport() const {
    return hwMeshExport_;
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

bool Options::isPostsmbExport() const {
    return postsmbExport_;
}

void Options::setPostmshExport(bool postmshExport) {
    postmshExport_ = postmshExport;
}

void Options::setPostsmbExport(bool postsmbExport) {
    postsmbExport_ = postsmbExport;
}

void Options::setContourRefinement(bool contourRefinement) {
    contourRefinement_ = contourRefinement;
}

void Options::setUnwantedConnectionsInfo(bool rhs) {
    unwantedConnectionsInfo_ = rhs;
}

void Options::setStructuredCellsInfo(bool rhs) {
    structuredCellsInfo_ = rhs;
}

void Options::setNfdeExport(bool rhs) {
    nfdeExport_ = rhs;
}

void Options::setHWMeshExport(bool hwMeshExport) {
    hwMeshExport_ = hwMeshExport;
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

Options::Mesher Options::strToMesher(std::string str) {
    if (str.compare("DMesher") == 0) {
        return Mesher::DMesher;
    } else if (str.compare("HMesher") == 0) {
        return Mesher::HMesher;
    } else {
        throw std::logic_error("Unreckognized label: " + str);
    }
}

Options::HMesherMode Options::strToHMesherMode(std::string str) {
    if (str.compare("Raw") == 0) {
        return HMesherMode::raw;
    }
    else if (str.compare("Adapted") == 0) {
        return HMesherMode::adapted;
    }
    else if (str.compare("Snap") == 0) {
        return HMesherMode::snap;
    }
    else {
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
    std::stringstream sts(str);
    Math::CVecR3 res;
    sts >> res(Math::Constants::x)
        >> res(Math::Constants::y)
        >> res(Math::Constants::z);
    return res;
}

std::pair<Math::CVecR3, Math::CVecR3> Options::strToBox(const std::string& value) {
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
