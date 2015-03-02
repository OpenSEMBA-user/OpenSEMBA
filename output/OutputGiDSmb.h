/*
 * OutputGiDSmb.h
 *
 *  Created on: Mar 2, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_OUTPUT_OUTPUTGIDSMB_H_
#define SRC_COMMON_OUTPUT_OUTPUTGIDSMB_H_

#include "OutputGiD.h"
#include "../BCGroup.h"
#include "../SmbData.h"

class OutputGiDSmb : public OutputGiD {
public:
    OutputGiDSmb();
    OutputGiDSmb(const SmbData* smb);
    OutputGiDSmb(const SmbData* smb, const string& fn);
    virtual ~OutputGiDSmb();
private:
    const SmbData* smb_;
    void writeMesh();
    void writeMeshWithIds(
            const vector<vector<uint> >& ids,
            string& name);
    void writeMeshWithIds(
            const vector<vector<uint> >& ids,
            const vector<string>& name);
    void writeMeshWithIds(
            const vector<uint>& ids, string& name);
    void writeOutputRequestsMesh();
    void writeMainMesh();
    void writeBCMesh();
    void writeBCMesh(
            const vector<const BoundaryCondition*>& list,
            const string& name,
            const CVecD3& colorRGB);
};

#endif /* SRC_COMMON_OUTPUT_OUTPUTGIDSMB_H_ */
