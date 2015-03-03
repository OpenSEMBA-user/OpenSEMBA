/*
 *  Created on: Aug 23, 2012
 *      Author: luis
 */
#include "OutputGiD.h"

int OutputGiD::numberOfOutputGiD_ = 0;
int OutputGiD::coordCounter_ = 0;
int OutputGiD::elemCounter_ = 0;

const CVecD3 OutputGiD::pecColor(255, 0, 0);
const CVecD3 OutputGiD::pmcColor(0, 255, 0);
const CVecD3 OutputGiD::smaColor(0, 0, 255);
const CVecD3 OutputGiD::pmlColor(0, 0, 255);
const CVecD3 OutputGiD::sibcColor(100, 0, 100);
const CVecD3 OutputGiD::emSourceColor(100, 100, 0);

OutputGiD::OutputGiD(const string& fn, GiD_PostMode mode) : Output(fn) {
    // Sets default values.
    mode_ = mode;
    meshFile_ = 0;
    resultFile_ = 0;
    if (numberOfOutputGiD_ == 0) {
        GiD_PostInit();
    }
    numberOfOutputGiD_++;
    // Opens files.
    deleteExistentOutputFiles();
    switch (mode_) {
    case GiD_PostAscii:
        openPostMeshFile(fn + ".post.msh");
        break;
    case GiD_PostBinary:
        openPostResultFile(fn + ".post.res");
        break;
    default:
        cerr << "ERROR @ GiDOutput::openFiles() " << endl;
    }
    writeGaussPoints();
}

OutputGiD::~OutputGiD() {
    switch (mode_) {
    case GiD_PostAscii:
        GiD_ClosePostMeshFile();
        break;
    default:
        GiD_ClosePostResultFile();
    }
    if (numberOfOutputGiD_ == 1) {
        GiD_PostDone();
    }
    numberOfOutputGiD_--;
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
        GiD_fBeginMesh(meshFile_, tName, dim, elementType, nNode);
    } else {
        GiD_fBeginMeshColor(meshFile_, tName, dim, elementType, nNode,
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
    const char *compv[cNames.size()];
    for (uint i = 0; i < cNames.size(); i++) {
        compv[i] = cNames[i].c_str();
//        compv[i] = new char[cNames[i].length() + 1];
//        strcpy(compv[i], cNames[i].c_str());
    }
    GiD_fBeginResult(resultFile_, fName, tName, time, resultType,
            getGiDResultLocation(), gpType, NULL, cNames.size(), compv);
}

void OutputGiD::openPostMeshFile(
        const string& filename) {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    meshFile_ = GiD_fOpenPostMeshFile(auxChar, mode_);
    delete [] auxChar;
}

void OutputGiD::openPostResultFile(
        const string& filename)  {
    char *auxChar;
    auxChar = new char[filename.length() + 1];
    strcpy(auxChar, filename.c_str());
    resultFile_ = GiD_fOpenPostResultFile(auxChar, mode_);
    if (mode_ == GiD_PostBinary) {
        meshFile_ = resultFile_;
    }
    delete [] auxChar;
}

void
OutputGiD::flushPostFile() const {
    if (meshFile_ != 0) {
        GiD_fFlushPostFile(meshFile_);
    }
    if (resultFile_ != 0) {
        GiD_fFlushPostFile(resultFile_);
    }
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
    GiD_fWriteCoordinates(meshFile_, id, pos(x), pos(y), pos(z));
}

void OutputGiD::writeCoordinates(const vector<CVecD3>& pos)  {
    GiD_fBeginCoordinates(meshFile_);
    for (uint i = 0; i < pos.size(); i++) {
        writeCoordinates(++coordCounter_, pos[i]);
    }
    GiD_fEndCoordinates(meshFile_);
}

void OutputGiD::writeElement(int elemId, int nId[]) const {
    GiD_fWriteElement(meshFile_, elemId, nId);
}

string OutputGiD::makeValid(string name) {
    name.erase(remove(name.begin(), name.end(), '\n'), name.end());
    return name;
}

void OutputGiD::beginCoordinates() const {
    GiD_fBeginCoordinates(meshFile_);
}

void OutputGiD::endCoordinates() const {
    GiD_fEndCoordinates(meshFile_);
}

void OutputGiD::beginElements() const {
    GiD_fBeginElements(meshFile_);
}

void OutputGiD::endElements() const {
    GiD_fEndElements(meshFile_);
}

void OutputGiD::endMesh() const {
    GiD_fEndMesh(meshFile_);
}
