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
