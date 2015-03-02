/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#include "OutputGiD.h"

const CVecD3 OutputGiD::pecColor(255, 0, 0);
const CVecD3 OutputGiD::pmcColor(0, 255, 0);
const CVecD3 OutputGiD::smaColor(0, 0, 255);
const CVecD3 OutputGiD::pmlColor(0, 0, 255);
const CVecD3 OutputGiD::sibcColor(100, 0, 100);
const CVecD3 OutputGiD::emSourceColor(100, 100, 0);

OutputGiD::OutputGiD() {
    initDefault();
}

OutputGiD::OutputGiD(const string& fn) : Output(fn) {
    initDefault();
    openGiDFiles();
}

OutputGiD::~OutputGiD() {
}

void OutputGiD::beginMesh(
        const string& name,
        GiD_Dimension dim,
        GiD_ElementType elementType,
        int nNode,
        const CVecD3& colorRGB) const {
    char *tName;
    tName = new char[name.length() + 1];
    strcpy(tName, name.c_str());
    if (colorRGB == CVecD3(0.0, 0.0, 0.0)) {
        GiD_BeginMesh(tName, dim, elementType, nNode);
    } else {
        GiD_BeginMeshColor(tName, dim, elementType, nNode,
                (float) colorRGB(0), (float) colorRGB(1), (float) colorRGB(2));
    }
    delete [] tName;
}

void OutputGiD::beginResult(
        const string& fieldName,
        const string& timeName,
        const double time,
        GiD_ResultType resultType,
        GiD_ResultLocation resultLocaltion,
        const string gaussPointType,
        const vector<string>& componentsNames) const {
    char fName[fieldName.length() + 1];
    strcpy(fName, fieldName.c_str());
    char tName[timeName.length() + 1];
    strcpy(tName, timeName.c_str());
    char gpType[gaussPointType.length() + 1];
    strcpy(gpType, gaussPointType.c_str());
    vector<string> cNames = componentsNames;
    char *compv[cNames.size()];
    for (uint i = 0; i < cNames.size(); i++) {
        compv[i] = new char[cNames[i].length() + 1];
        strcpy(compv[i], cNames[i].c_str());
    }
    GiD_BeginResult(fName, tName, time, resultType,
            getGiDResultLocation(), gpType, NULL, cNames.size(), compv);
}

void OutputGiD::openPostMeshFile(
        const string& filename,
        const GiD_PostMode mode) const {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    GiD_OpenPostMeshFile(auxChar, mode);
    delete [] auxChar;
}

void OutputGiD::openPostResultFile(
        const string& filename,
        const GiD_PostMode mode) const {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    GiD_OpenPostResultFile(auxChar, mode);
    delete [] auxChar;
}

void
OutputGiD::flushPostFile() const {
    GiD_FlushPostFile();
}


void
OutputGiD::writeGaussPoints() const {
    //    {
    //        const char* name = "gp_tri";
    //        static const SimplexTri<ORDER_N> tri;
    //        GiD_BeginGaussPoint((char*) name, GiD_Triangle, NULL,tri.np,0,0);
    //        for (uint i = 0; i < tri.np; i++) {
    //            CVecD3 pos = tri.coordinate(i);
    //            GiD_WriteGaussPoint2D(pos(1), pos(2));
    //        }
    //        GiD_EndGaussPoint();
    //    }
    //    {
    //        const char* name = "gp_tet";
    //        static const SimplexTet<ORDER_N> tet;
    //        GiD_BeginGaussPoint((char*) name, GiD_Tetrahedra, NULL,tet.np,0,0);
    //        for (uint i = 0; i < tet.np; i++) {
    //            CartesianVector<double,4> pos = tet.coordinate(i);
    //            GiD_WriteGaussPoint3D(pos(1), pos(2), pos(3));
    //        }
    //        GiD_EndGaussPoint();
    //    }
}

GiD_ResultType OutputGiD::getGiDResultType(
        OutputRequest::Type type) const {
    switch (type) {
    case OutputRequest::electricField:
        return GiD_ResultType::GiD_Vector;
    case OutputRequest::magneticField:
        return GiD_ResultType::GiD_Vector;
    default:
        assert(false);
        return GiD_ResultType::GiD_Scalar;
    }
}

GiD_ResultLocation OutputGiD::getGiDResultLocation() const {
    return GiD_ResultLocation::GiD_OnNodes;
}


void OutputGiD::writeCoordinates(const uint id, const CVecD3 pos) const {
    GiD_WriteCoordinates(id, pos(x), pos(y), pos(z));
}

void OutputGiD::writeCoordinates(const vector<CVecD3>& pos)  {
    GiD_BeginCoordinates();
    for (uint i = 0; i < pos.size(); i++) {
        GiD_WriteCoordinates(++coordCounter_, pos[i](x), pos[i](y), pos[i](z));
    }
    GiD_EndCoordinates();
}

void OutputGiD::initDefault() {
    mode_ = GiD_PostAscii;
    coordCounter_ = 0;
    elemCounter_ = 0;
}

void OutputGiD::openGiDFiles() {
    deleteExistentOutputFiles();
    switch (mode_) {
    case GiD_PostAscii:
        openPostMeshFile(getOutputfilename() + ".post.msh", mode_);
        //        openPostResultFile(getOutputfilename() + ".post.res", mode_);
        break;
    case GiD_PostBinary:
        openPostResultFile(getOutputfilename() + ".post.res", mode_);
        break;
    default:
        cerr << "ERROR @ GiDOutput::openFiles() " << endl;
    }
    writeGaussPoints();
}

string OutputGiD::makeValid(string name) {
    name.erase(remove(name.begin(), name.end(), '\n'), name.end());
    return name;
}
