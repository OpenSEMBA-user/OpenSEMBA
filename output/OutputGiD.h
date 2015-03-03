/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#ifndef OUTPUTGID_H_
#define OUTPUTGID_H_

#include "Output.h"

#include "../gidpost/gidpost.h"
#include "../SmbData.h"

using namespace std;

class OutputGiD : public Output {
public:
    OutputGiD(const string& filename, GiD_PostMode mode = GiD_PostAscii);
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
    void openPostMeshFile(const string& filename);
    void openPostResultFile(const string& filename);
    void flushPostFile() const;
    void beginCoordinates() const;
    void writeGaussPoints() const;
    void writeCoordinates(const uint id, const CVecD3 pos) const;
    void writeCoordinates(const vector<CVecD3>& pos);
    void endCoordinates() const;
    void beginElements() const;
    void writeElement(int elemId, int nId[]) const;
    void endElements() const;
    void endMesh() const;
    GiD_ResultType getGiDResultType(OutputRequest::Type type) const;
    GiD_ResultLocation getGiDResultLocation() const;
protected:
    int coordCounter_;
    int elemCounter_;
    static const CVecD3 pecColor, pmcColor, smaColor, pmlColor,
     sibcColor, emSourceColor;
    static string makeValid(string name);
private:
    static int numberOfFiles_;
    GiD_FILE meshFile_;
    GiD_FILE resultFile_;
    GiD_PostMode mode_;
};

#endif /* GIDOUTPUT_H_ */
