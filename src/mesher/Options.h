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
        openfoam,
        conformalMesher,
        zMesher,
        none
    };
    enum class Mode {
        structured,
        relaxed,
        slanted,
        conformal
    };

    Options();

    void addArguments(Argument::Group& args) const;
    void set(const Solver::Settings& opts);

    Mesher getMesher() const;
    virtual const Math::CVecR3& getLocationInMesh() const;
    virtual bool isLocationInMeshSet() const;
    Mode getMode() const;
    bool isStructured() const;
    bool isRelaxed() const;
    bool isBruteForceVolumes() const;
    Math::Real getForbiddenLength() const;
    Math::Real getScalingFactor() const;
    std::string getScaleFactor() const;
    const std::string& getMeshOutputName() const;
    bool isGridStepSet() const;
    bool isVtkExport() const;
    bool isPostmshExport() const;
    const Math::CVecR3& getGridStep() const;
    const std::pair<Math::CVecR3, Math::CVecR3>& getBoundaryMeshSize() const;
    const PhysicalModel::Bound::Bound* getBoundTermination(const std::size_t d,
                                                    const std::size_t p) const;
    const Geometry::BoundTerminations3& getBoundTerminations() const;

    const std::string& getOutputName() const;

    void setBoundaryMeshSize(const std::pair<Math::CVecR3, Math::CVecR3>&);
    void setBoundTermination(const std::size_t d,
                             const std::size_t p,
                             const PhysicalModel::Bound::Bound*);
    void setBruteForceVolumes(bool bruteForceVolumes);
    void setForbiddenLength(const Math::Real& edgeFraction);
    void setGridStep(const Math::CVecR3& gridStep);
    void setLocationInMesh(const Math::CVecR3& locationInMesh);
    void setLocationInMeshSet(bool locationInMeshSet);
    void setMesher(Mesher mesher);
    void setMode(Mode mode);
    void setPostmshExport(bool postmshExport);
    void setOutputName(const std::string& outputName);
    void setScaleFactorValue(const std::string& scaleFactorValue);
    void setScalingFactor(const Math::Real& scalingFactor);
    void setVtkExport(bool vtkExport);

    void applyGeometricScalingFactor(const Math::Real& factor);

    void printHelp() const;
    void printInfo() const;

    static std::string toStr(const PhysicalModel::Bound::Bound*);

private:
    Mesher mesher_;
    Math::Real scalingFactor_;
    Mode mode_;
    bool bruteForceVolumes_;
    Math::Real forbiddenLength_;
    std::string scaleFactorValue_;
    bool locationInMeshSet_;
    Math::CVecR3 locationInMesh_;
    std::string outputName_;

    bool vtkExport_;
    bool postmshExport_;
    Math::CVecR3 gridStep_;
    Geometry::BoundTerminations3 boundTermination_;

    static Mesher strToMesher(std::string);
    static Mode strToMesherMode(std::string);
    static Math::CVecR3 strToCVecR3(const std::string& str);
    static std::pair<Math::CVecR3, Math::CVecR3> strToBox(
            const std::string& str);
    static const PhysicalModel::Bound::Bound* strToBoundType(std::string label);
};

}
}

#endif /* SEMBA_MESHER_OPTIONS_H_ */
