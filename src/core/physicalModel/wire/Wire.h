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

#ifndef SEMBA_PHYSICALMODEL_WIRE_H_
#define SEMBA_PHYSICALMODEL_WIRE_H_

#include "physicalModel/PhysicalModel.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

class Wire : public virtual PhysicalModel {
public:
    Wire(const Id id,
         const std::string name,
         const Math::Real radius,
         const Math::Real resistance,
         const Math::Real inductance);
    Wire(const Id id,
         const std::string name,
         const Math::Real radius,
         const Math::Real resistance,
         const Math::Real inductance,
         const Math::Real capacitance,
         const Math::Real pResistance,
         const Math::Real pInductance,
         const Math::Real pCapacitance);
    Wire(const Id id,
         const std::string name,
         const Math::Real radius,
         const std::string filename);
    Wire(const Wire&);
    virtual ~Wire();

    SEMBA_CLASS_DEFINE_CLONE(Wire);

    Math::Real getRadius() const;

    bool isSeriesParallel() const;
    bool isDispersive() const;

    Math::Real getSeriesResistance() const;
    Math::Real getSeriesInductance() const;
    Math::Real getSeriesCapacitance() const;
    Math::Real getParallelResistance() const;
    Math::Real getParallelInductance() const;
    Math::Real getParallelCapacitance() const;

    std::string getFilename() const;

    void printInfo() const;

private:
    Math::Real radius_;
    bool isSeriesParallel_;
    bool isDispersive_;
    Math::Real seriesResistance_;    // Resistance per meter.
    Math::Real seriesInductance_;    // Inductance per meter.
    Math::Real seriesCapacitance_;   // Capacitance per meter.
    Math::Real parallelResistance_;  // Resistance per meter.
    Math::Real parallelInductance_;  // Inductance per meter.
    Math::Real parallelCapacitance_; // Capacitance per meter.
    std::string filename_;
};

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_WIRE_H_ */
