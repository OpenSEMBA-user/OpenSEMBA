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
#ifndef SEMBA_MESHER_OPTIONS_H_
#define SEMBA_MESHER_OPTIONS_H_

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

    void printHelp() const;
    virtual void printInfo() const;

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

#endif /* SEMBA_MESHER_OPTIONS_H_ */
