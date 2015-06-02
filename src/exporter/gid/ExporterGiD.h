/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef EXPORTER_GID_EXPORTERGID_H_
#define EXPORTER_GID_EXPORTERGID_H_

#include "exporter/Exporter.h"
#include "SmbData.h"
#include "gidpost/gidpost.h"

using namespace std;

class ExporterGiD : public Exporter {
public:
    ExporterGiD(
            const SmbData* smb,
            GiD_PostMode mode = GiD_PostAscii);
    ExporterGiD(
            const SmbData* smb,
            const string& fn,
            GiD_PostMode mode = GiD_PostAscii);
//    ExporterGiD(
//            const MeshUnstructured* mesh,
//            const GroupPhysicalModels<>* mat,
//            const string& fn,
//            GiD_PostMode mode = GiD_PostAscii);
    virtual ~ExporterGiD();
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
            const GroupPhysicalModels<>* mat,
            const GroupEMSources<>* src = NULL,
            const GroupOutRqs<>* oRq = NULL);
    void writeBoundaries(const BoxR3 box, const OptionsMesher* opts);
    void writeElements(
            const GroupElements<const ElemR>& entities,
            const string& name,
            const GiD_ElementType type,
            const Int nV);
    static string makeValid(string name);
    void beginCoordinates() const;
    void writeGaussPoints() const;
    void writeCoordinates(CoordR3Group& pos);
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
    void writeAllElements(const GroupElements<const ElemR>& elem,
                          const string& name);
    void initDefault(const SmbData* smb, GiD_PostMode mode, const string& fn);
};

#endif /* EXPORTER_GID_EXPORTERGID_H_ */
