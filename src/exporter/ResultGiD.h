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
 * ResultGiD.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */
#ifndef RESULTGID_H_
#define RESULTGID_H_

#include <string.h>

using namespace std;

#include "outRq/OutRq.h"
#include "exporter/gid/ExporterGiD.h"
#include "exporter/Result.h"
//
//class ResultGiD : public Result, public OutRq {
//    friend class ExporterGiD;
//public:
//    ResultGiD();
//    ResultGiD(
//            const OutRq* p,
//            Int& coordCounter,
//            Int& elemCounter,
//            const DG* dg,
//            const Mesh* mesh);
//    virtual ~ResultGiD();
//    void writePointProbeMesh(Int& coordCounter, Int& elemCounter);
//    void writeTriProbeMesh(Int& coordCounter, Int& elemCounter);
//    void writeTetProbeMesh(Int& coordCounter, Int& elemCounter);
//    void
//    write(const Real time,
//            const FieldR3& electric,
//            const FieldR3& magnetic) const;
//private:
//    vector<UInt> coord_;
//    vector<UInt> solverNode_;
//    const DG* dg_;
//    const Mesh* mesh_;
//    CVecR3 getOutputValueFromFields(
//            const CVecR3& electric,
//            const CVecR3& magnetic) const;
//    string getGiDGaussPointType() const;
//    vector<string> getComponentNames() const;
//};

#endif /* GIDRESULT_H_ */
