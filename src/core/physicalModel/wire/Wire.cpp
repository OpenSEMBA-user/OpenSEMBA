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

#include "Wire.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

Wire::Wire(const Id id,
           const std::string name,
           const Math::Real radius,
           const Math::Real resistance,
           const Math::Real inductance)
:   Identifiable<Id>(id), 
    PhysicalModel(name) {
    radius_ = radius;
    isSeriesParallel_ = false;
    isDispersive_ = false;
    seriesResistance_ = resistance;
    seriesInductance_ = inductance;
    seriesCapacitance_ = 0.0;
    parallelResistance_ = 0.0;
    parallelInductance_ = 0.0;
    parallelCapacitance_ = 0.0;
}

Wire::Wire(const Id id,
           const std::string name,
           const Math::Real radius,
           const Math::Real resistance,
           const Math::Real inductance,
           const Math::Real capacitance,
           const Math::Real pResistance,
           const Math::Real pInductance,
           const Math::Real pCapacitance)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    radius_ = radius;
    isSeriesParallel_ = true;
    isDispersive_ = false;
    seriesResistance_ = resistance;
    seriesInductance_ = inductance;
    seriesCapacitance_ = capacitance;
    parallelResistance_ = pResistance;
    parallelInductance_ = pInductance;
    parallelCapacitance_ = pCapacitance;
}

Wire::Wire(const Id id,
           const std::string name,
           const Math::Real radius,
           const std::string filename)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    radius_ = radius;
    isSeriesParallel_ = false;
    isDispersive_ = true;
    seriesResistance_ = 0.0;
    seriesInductance_ = 0.0;
    seriesCapacitance_ = 0.0;
    parallelResistance_ = 0.0;
    parallelInductance_ = 0.0;
    parallelCapacitance_ = 0.0;
    filename_ = filename;
}

Wire::Wire(const Wire& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    radius_ = rhs.radius_;
    isSeriesParallel_ = rhs.isSeriesParallel_;
    isDispersive_ = rhs.isDispersive_;
    seriesResistance_ = rhs.seriesResistance_;
    seriesInductance_ = rhs.seriesInductance_;
    seriesCapacitance_ = rhs.seriesCapacitance_;
    parallelResistance_ = rhs.parallelResistance_;
    parallelInductance_ = rhs.parallelInductance_;
    parallelCapacitance_ = rhs.parallelCapacitance_;
    filename_ = rhs.filename_;
}

Wire::~Wire() {

}

bool Wire::isSeriesParallel() const {
    return isSeriesParallel_;
}

bool Wire::isDispersive() const {
    return isDispersive_;
}

Math::Real Wire::getRadius() const {
    return radius_;
}

Math::Real Wire::getSeriesResistance() const {
    return seriesResistance_;
}

Math::Real Wire::getSeriesInductance() const {
    return seriesInductance_;
}

Math::Real Wire::getSeriesCapacitance() const {
    return seriesCapacitance_;
}

Math::Real Wire::getParallelResistance() const {
    return parallelResistance_;
}

Math::Real Wire::getParallelInductance() const {
    return parallelInductance_;
}

Math::Real Wire::getParallelCapacitance() const {
    return parallelCapacitance_;
}

std::string Wire::getFilename() const {
    return filename_;
}


void Wire::printInfo() const {
    std::cout<< " --- Wire info ---" << std::endl;
    PhysicalModel::printInfo();
    std::cout << " Radius: " << radius_ << std::endl;
    if (isDispersive_) {
        std::cout << " Dispersive Filename: " << filename_ << std::endl;
    } else if (isSeriesParallel_) {
        std::cout << " Series Resistance: " << seriesResistance_ << std::endl
                  << " Series Inductance: " << seriesInductance_ << std::endl
                  << " Series Capacitance: " << seriesCapacitance_ << std::endl
                  << " Parallel Resistance: " << parallelResistance_
                  << std::endl
                  << " Parallel Inductance: " << parallelInductance_
                  << std::endl
                  << " Parallel Capacitance: " << parallelCapacitance_
                  << std::endl;
    } else {
        std::cout << " Resistance: " << seriesResistance_ << std::endl
                  << " Inductance: " << seriesInductance_ << std::endl;
    }
}

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
