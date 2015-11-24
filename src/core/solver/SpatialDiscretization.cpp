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
 * SpatialDiscretization.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: luis
 */

#include "SpatialDiscretization.h"

SpatialDiscretization::SpatialDiscretization() {
    outRqs_ = NULL;
}

SpatialDiscretization::~SpatialDiscretization() {

}

void SpatialDiscretization::setOutputRequests(const GroupOutRqs<>* outRqs) {
    // Builds index.
    for (UInt i = 0; i < outRqs->size(); i++) {

        // TODO Build index.

    }
}

//GroupOutputs SpatialDiscretization::getOutputs() const {
//    GroupOutputs res;
//
//    for (UInt i = 0; i < outRqs_->size(); i++) {
//        multimap<ElementId,vector<CVecR3>> electric, magnetic;
//        const OutRqBase* outRq = outRqs_(i);
//        for (UInt j = 0; j < outRq->elems().size(); j++) {
//            const Elem* elem = outRq->elems()(i);
//            const ElementId id = elem->getId();
//            vector<const Cell*> cell = outputIdsToCells_.find(id);
//            for (UInt k = 0; k < cell.size(); k++) {
////                const UInt outputsInterpolationOrder = cell[k]->getOrder();
//                vector <CVecR3> gaussPos =
//                        elem->getGaussPoints(outputsInterpolationOrder);
//                electric.insert(pair<ElementId,vector<CVecR3>>(
//                        id, cell[k]->getFieldAt(gaussPos, Cell::FieldType::electric)));
//                magnetic.insert(pair<ElementId,vector<CVecR3>>(
//                        id, cell[k]->getFieldAt(gaussPos, Cell::FieldType::magnetic)));
//            }
//        }
//
//        res.add(outRq, electric, magnetic);
//
//    }
//    return res;
//}
