/*
 * ResultGiD.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */
#ifndef RESULTGID_H_
#define RESULTGID_H_

#include "../../../common/inputs/OutputRequest.h"
#include "../../../common/gidpost/gidpost.h"
#include "OutputGiD.h"
#include <string.h>
#include "../../../common/exporter/Result.h"

using namespace std;

class ResultGiD : public Result, public OutRq {
    friend class OutputGiD;
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
            const FieldD3& electric,
            const FieldD3& magnetic) const;
private:
    vector<uint> coord_;
    vector<uint> solverNode_;
    const DG* dg_;
    const Mesh* mesh_;
    CVecD3 getOutputValueFromFields(
            const CVecD3& electric,
            const CVecD3& magnetic) const;
    string getGiDGaussPointType() const;
    vector<string> getComponentNames() const;
};

#endif /* GIDRESULT_H_ */
