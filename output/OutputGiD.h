/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef OUTPUTGID_H_
#define OUTPUTGID_H_

#include "Output.h"

#include "../BCGroup.h"
#include "../SmbData.h"
#include "../NFDEData.h"
#include "../gidpost/gidpost.h"

using namespace std;

class OutputGiD : public Output {
public:
    OutputGiD();
    OutputGiD(
            const SmbData* smb);
    OutputGiD(
            const NFDEData* data);
    virtual ~OutputGiD();
protected:
    void beginMesh(
            const string& tName,
            GiD_Dimension dim,
            GiD_ElementType elementType,
            int nNode,
            const CVecD3& ColorRGB = CVecD3()) const;
    void beginResult(
            const string& fieldName,
            const string& timeName,
            const double time,
            GiD_ResultType resultType,
            GiD_ResultLocation resultLocaltion,
            const string gaussPointType,
            const vector<string>& componentsNames) const;
    void openPostMeshFile(
            const string& filename,
            const GiD_PostMode mode) const;
    void openPostResultFile(
            const string& filename,
            const GiD_PostMode mode) const;
    void flushPostFile() const;
    void writeGaussPoints() const;
    GiD_ResultType getGiDResultType(OutputRequest::Type type) const;
    GiD_ResultLocation getGiDResultLocation() const;
private:
    const SmbData* smb_;
    const NFDEData* nfde_;
    GiD_PostMode mode_;
    int coordCounter_;
    int elemCounter_;
    void openGiDFiles();
    // SmbData stuff...
    void writeMesh();
    void writeMeshWithIds(
            const vector<vector<uint> >& ids,
            string& name);
    void writeMeshWithIds(
            const vector<vector<uint> >& ids,
            const vector<string>& name);
    void writeMeshWithIds(
            const vector<uint>& ids, string& name);
    void writeQuad4Mesh(
            const string& name,
            const MeshVolume& mesh);
    void writeHex8Mesh(
            const string& name,
            const MeshVolume& mesh);
    void writeOutputRequestsMesh();
    void writeMainMesh();
    void writeBCMesh();
    void writeBCMesh(
            const vector<const BoundaryCondition*>& list,
            const string& name,
            const CVecD3& colorRGB);
    // NFDEData stuff...
    void writeBoundaries() const;
};

#endif /* GIDOUTPUT_H_ */
