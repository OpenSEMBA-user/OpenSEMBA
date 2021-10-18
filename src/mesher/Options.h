
#pragma once

#include <array>
#include <cstdlib>
#include <memory>
#include <string>
#include <assert.h>

#include "argument/Parser.h"
#include "math/vector/Cartesian.h"
#include "geometry/Grid.h"
#include "geometry/BoundTerminations.h"
#include "physicalModel/bound/Bound.h"
#include "physicalModel/bound/PML.h"
#include "physicalModel/bound/PMC.h"
#include "physicalModel/bound/PEC.h"
#include "physicalModel/bound/Mur1.h"
#include "physicalModel/bound/Mur2.h"
#include "physicalModel/bound/Periodic.h"
#include "solver/Info.h"

namespace SEMBA {
namespace Mesher {

class Options {
public:
    enum class Mesher {
        DMesher,
        HMesher
    };

    enum class Mode {
        structured,
        relaxed,
        slanted,
        conformal
    };

    enum class HMesherMode {
        raw,
        adapted,
        snap
    };

    Options();

    void addArguments(Argument::Group& args) const;
    void set(const Solver::Settings& opts);

    Mesher getMesher() const;
    HMesherMode getHMesherMode() const;
    Mode getMode() const;
    bool isStructured() const;
    bool isRelaxed() const;
    Math::Int getEdgePoints() const;
    Math::Real getForbiddenLength() const;
    Math::Real getScalingFactor() const;
    bool isGridStepSet() const;

    bool isNfdeExport() const;
    bool isHWMeshExport() const;
    bool isPostmshExport() const;
    bool isPostsmbExport() const;
    bool isVtkExport() const;

    bool isContourRefinement() const;
    bool isUnwantedConnectionsInfo() const;
    bool isStructuredCellsInfo() const;
    const Math::CVecR3& getGridStep() const;
    const PhysicalModel::Bound::Bound* getBoundTermination(const std::size_t d,
                                                    const std::size_t p) const;
    const Geometry::BoundTerminations3& getBoundTerminations() const;

    bool       isSlanted() const;
    Math::Real getSlantedThreshold() const;

    const std::string& getOutputName() const;

    void setMesher(const Mesher);
    void setHMesherMode(const HMesherMode);
    void setBoundTermination(const std::size_t d,
                             const std::size_t p,
                             const PhysicalModel::Bound::Bound*);
    void setEdgePoints(const Math::Int&);
    void setForbiddenLength(const Math::Real& edgeFraction);
    void setGridStep(const Math::CVecR3& gridStep);
    void setMode(const Mode mode);
    
    void setNfdeExport(bool);
    void setHWMeshExport(bool);
    void setPostmshExport(bool postmshExport);
    void setPostsmbExport(bool postmshExport);
    void setVtkExport(bool vtkExport);

    void setOutputName(const std::string& outputName);
    void setScalingFactor(const Math::Real& scalingFactor);
    void setContourRefinement(bool contourRefinement);
    void setUnwantedConnectionsInfo(bool rhs);
    void setStructuredCellsInfo(bool rhs);

    void setSlanted(const bool&);
    void setSlantedThreshold(const Math::Real&);

    static std::string toStr(const PhysicalModel::Bound::Bound*);
    
private:
    Mesher mesher_;
    
    Math::UInt edgePoints_;
    Mode mode_;
    HMesherMode hMesherMode_;
    bool snap_;
    Math::Real forbiddenLength_;
    std::string scaleFactorValue_;
    
    std::string outputName_;
    Math::Real scalingFactor_;
    bool contourRefinement_;
    bool unwantedConnectionsInfo_;
    bool structuredCellsInfo_;
    
    bool hwMeshExport_;
    bool nfdeExport_;
    bool postmshExport_;
    bool postsmbExport_;
    bool vtkExport_;

    Math::CVecR3 gridStep_;

    Geometry::BoundTerminations3 boundTermination_;

    bool slanted_;
    Math::Real slantedThreshold_;

    static Mesher strToMesher(std::string);
    static Mode strToMesherMode(std::string);
    static HMesherMode strToHMesherMode(std::string);
    static Math::CVecR3 strToCVecR3(const std::string& str);
    static std::pair<Math::CVecR3, Math::CVecR3> strToBox(
            const std::string& str);
    static const PhysicalModel::Bound::Bound* strToBoundType(std::string);
};

}
}

