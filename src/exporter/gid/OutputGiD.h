/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef OUTPUTGID_H_
#define OUTPUTGID_H_

#include "exporter/Output.h"
#include "SmbData.h"
#include "gidpost/gidpost.h"

using namespace std;

class OutputGiD : public Output {
public:
    OutputGiD(
            const SmbData* smb,
            GiD_PostMode mode = GiD_PostAscii);
    OutputGiD(
            const SmbData* smb,
            const string& fn,
            GiD_PostMode mode = GiD_PostAscii);
    OutputGiD(
            const MeshUnstructured* mesh,
            const PhysicalModelGroup<>* mat,
            const string& fn,
            GiD_PostMode mode = GiD_PostAscii);
    virtual ~OutputGiD();
protected:
    void beginMesh(
            const string& tName,
            GiD_Dimension dim,
            GiD_ElementType elementType,
            Int nNode,
            const CVecR3& ColorRGB = CVecR3()) const;
    void beginResult(
            const string& fieldName,
            const string& timeName,
            const Real time,
            GiD_ResultType resultType,
            GiD_ResultLocation resultLocaltion,
            const string gaussPointType,
            const vector<string>& componentsNames) const;
    void flushPostFile() const;
private:
    static Int coordCounter_;
    static Int elemCounter_;
    static const CVecR3 pecColor, pmcColor, smaColor, pmlColor,
     sibcColor, emSourceColor;
    static Int numberOfOutputGiD_;
    GiD_FILE meshFile_;
    GiD_FILE resultFile_;
    GiD_PostMode mode_;
    void writeMesh(
            const Mesh* inMesh,
            const PhysicalModelGroup<>* mat,
            const EMSourceGroup<>* src = NULL,
            const OutRqGroup<>* oRq = NULL);
//    void writeMeshWithIds(
//            const vector<vector<ElementId> >& ids,
//            string& name);
//    void writeMeshWithIds(
//            const vector<vector<ElementId> >& ids,
//            const vector<string>& name);
//    void writeMeshWithIds(
//            const vector<ElementId>& ids, string& name);
//    void writeOutputRequestsMesh();
    void writeElements(
            const ElementsGroup<const ElemR>& entities,
            const string& name,
            const GiD_ElementType type,
            const Int nV);
    static string makeValid(string name);
    void beginCoordinates() const;
    void writeGaussPoints() const;
    void writeCoordinates(const UInt id, const CVecR3 pos) const;
    void writeCoordinates(const vector<CVecR3>& pos);
    void endCoordinates() const;
    void beginElements() const;
    void writeElement(Int elemId, int nId[]) const;
    void endElements() const;
    void endMesh() const;
    GiD_ResultType getGiDResultType(OutRqBase::Type type) const;
    GiD_ResultLocation getGiDResultLocation() const;
    void openPostMeshFile(const string& filename);
    void openPostResultFile(const string& filename);
    void writeMaterialsInLayer(const Layer* lay);
    void writeAllElements(const ElementsGroup<const ElemR>& elem,
                          const string& name);
    void initDefault(GiD_PostMode mode, const string& fn);
};

#endif /* GIDOUTPUT_H_ */
