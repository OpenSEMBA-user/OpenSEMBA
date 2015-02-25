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
#include "../output/OutputNFDE.h"
#include "../gidpost/gidpost.h"

using namespace std;

class OutputGiD : public Output {
    friend class OutputNFDE;
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
    void writeCoordinates(const uint id, const CVecD3 pos) const;
    void writeCoordinates(const vector<CVecD3>& pos);
    GiD_ResultType getGiDResultType(OutputRequest::Type type) const;
    GiD_ResultLocation getGiDResultLocation() const;
private:
    const SmbData* smb_;
    const NFDEData* nfde_;
    GiD_PostMode mode_;
    int coordCounter_;
    int elemCounter_;
    static const CVecD3 pecColor, pmcColor, smaColor, pmlColor,
     sibcColor, emSourceColor;
    void openGiDFiles();
    static string makeValid(string name);
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
    void writeOutputRequestsMesh();
    void writeMainMesh();
    void writeBCMesh();
    void writeBCMesh(
            const vector<const BoundaryCondition*>& list,
            const string& name,
            const CVecD3& colorRGB);
    // NFDEData stuff...
    void writeSpaceSteps();
    void writeBoundaries();
    void writePlaneWaveSource();
    void writeCurrentDensitySource();
    void writeFieldSource();
    void writeLine(const NFDEData::Line*);
    void writeSurf(const NFDEData::Surf*);
    void writeBody(const NFDEData::Body*);
    void writeThinWire();
    void writeThinGap();
    void writeTraditionalProbe();
    void writeNewProbe();
    void writeBulkProbes();
    void writeSliceProbes();
    void writeCoordMultiplier(
                const vector<const NFDEData::CoordsMultiplier*>& entities,
                const string& name);
    void writeCoordNodes(
            const vector<const NFDEData::CoordsNode*>& entities,
            const string& name);
    void writeCoordLines(
            const vector<const NFDEData::CoordsLine*>& entities,
            const string& name);
};

#endif /* GIDOUTPUT_H_ */
