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

#ifndef COMMON_EXPORTER_EXPORTER_H_
#define COMMON_EXPORTER_EXPORTER_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Data.h"
#include "geometry/mesh/Structured.h"

namespace SEMBA {
namespace Exporter {

class Exporter : public FileSystem::Project {
public:
    Exporter();
    Exporter(const std::string& name);
    virtual ~Exporter();

//    virtual void process(
//            const Real time,
//            const Group<Output>& outputs) = 0;
//    virtual void setOutputs(
//            const Group<Output>& outputs) = 0;

    virtual void printInfo() const;

protected:
    void deleteExistentOutputFiles() const;
    Size determineStepsSaved(
            const Math::Real savingPeriod,
            const Math::Real dt) const;
protected:
    std::string getOutputfilename() const;
    Group::Group<Geometry::ElemR> getBoundary(
            const Math::Constants::CartesianAxis dir,
            const Math::Constants::CartesianBound bound,
            Geometry::CoordR3Group& cG,
            const Geometry::Grid3* grid,
            const Geometry::Mesh::Mesh* mesh) const;
    Group::Group<Geometry::ElemR> getGridElems(
            Geometry::CoordR3Group& cG,
            const Geometry::Grid3* grid) const;
    static std::string getBoundaryName(
            const Geometry::Mesh::Structured* mesh,
            const Size i,
            const Size j);
private:
    //   void writeAllFields(const FieldR3& field) const;
};

} /* namespace Exporter */
} /* namespace SEMBA */

#endif /* COMMON_EXPORTER_EXPORTER_H_ */
