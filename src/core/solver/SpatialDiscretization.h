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
 * SpatialDiscretization.h
 *
 *  Created on: Nov 19, 2015
 *      Author: luis
 */

#ifndef SRC_CORE_SOLVER_SPATIALDISCRETIZATION_H_
#define SRC_CORE_SOLVER_SPATIALDISCRETIZATION_H_

#include <map>
using namespace std;

#include "base/group/Group.h"
#include "outRq/GroupOutRqs.h"
#include "exporter/GroupOutputs.h"
#include "Cell.h"
//
//class SpatialDiscretization {
//public:
//    SpatialDiscretization();
//    virtual ~SpatialDiscretization();
//
//    void setOutputRequests(const GroupOutRqs<>* outRqs);
//
//    GroupOutputs<> getOutputs() const;
//
//protected:
//    const GroupOutRqs<>* outRqs_;
//
//    vector<const FieldBase*> getFieldsAt_(
//            const vector<CVecR3>& position,
//            const bool indexPosition = false) const;
//    //!< Returns all fields in a set of positions.
//
//
//    map<ElementId,vector<const Cell*>> posToCells_;
//    //!< Index of positions in to Solver cells.
//};

#endif /* SRC_CORE_SOLVER_SPATIALDISCRETIZATION_H_ */
