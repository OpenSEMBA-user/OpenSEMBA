/*
 * ResultGiD.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */
#ifndef RESULTGID_H_
#define RESULTGID_H_

#include "outRq/OutRq.h"
#include "../../../common/gidpost/gidpost.h"
#include "OutputGiD.h"
#include <string.h>
#include "../../../common/exporter/Result.h"

using namespace std;

class ResultGiD : public Result, public OutRq {
    friend class ExporterGiD;
public:
    ResultGiD();
    ResultGiD(
            const OutRq* p,
            int& coordCounter,
            int& elemCounter,
            const DG* dg,
            const Mesh* mesh);
    virtual ~ResultGiD();
    void writePointProbeMesh(int& coordCounter, int& elemCounter);
    void writeTriProbeMesh(int& coordCounter, int& elemCounter);
    void writeTetProbeMesh(int& coordCounter, int& elemCounter);
    void
    write(const double time,
            const FieldR3& electric,
            const FieldR3& magnetic) const;
private:
    vector<uint> coord_;
    vector<uint> solverNode_;
    const DG* dg_;
    const Mesh* mesh_;
    CVecR3 getOutputValueFromFields(
            const CVecR3& electric,
            const CVecR3& magnetic) const;
    string getGiDGaussPointType() const;
    vector<string> getComponentNames() const;
};

#endif /* GIDRESULT_H_ */
