#ifndef OUTPUT_H_
#include "Output.h"
#endif

Output::Output()  {
}

Output::Output(const string& name) : ProjectFile(name) {

}

Output::~Output() {
    for (uint i = 0; i < result_.size(); i++) {
        delete result_[i];
    }
}

void
Output::writeResumeFile(
        const double time, const FieldD3& electric, const FieldD3& magnetic) {
    string auxStr = getProjectFolder() + getProjectName() + ".resume";
    char *auxChar;
    auxChar = new char[auxStr.length() + 1];
    strcpy(auxChar, auxStr.c_str());
    GiD_OpenPostResultFile(auxChar, GiD_PostAscii);
    {
        const char * compv[] = {"Ex", "Ey", "Ez", "|E|"};
        const char * fName = "Electric Field";
        const char * tName = "Time";
        GiD_BeginResult(fName, tName, time,
                GiD_Vector, GiD_OnNodes, NULL, NULL, 4, compv);
        writeAllFields(electric);
        GiD_EndResult();
    }
    {
        const char * compv[] = {"Hx", "Hy", "Hz", "|H|"};
        const char * fName = "Magnetic Field";
        const char * tName = "Time";
        GiD_BeginResult(fName, tName, time,
                GiD_Vector, GiD_OnNodes, NULL, NULL, 4, compv);
        writeAllFields(magnetic);
        GiD_EndResult();
    }
    GiD_ClosePostResultFile();
    cout << " - File for resuming was succesfully written." << endl;
}

void
Output::process(
        const double time,
        const FieldD3& electric,
        const FieldD3& magnetic) {
    for (uint i = 0; i < result_.size(); i++) {
        result_[i]->write(time, electric, magnetic);
    }
}

void
Output::deleteExistentOutputFiles() const {
    string fileName = getProjectFolder() + getProjectName();
    string cmd;
    cmd = "rm -f " + fileName + ".post.msh";
    if (system(cmd.c_str())) {
        cerr << "ERROR @ Output: "
                << "System command failed to execute: " << cmd << endl;
    }
    cmd = "rm -f " + fileName + ".post.res";
    if (system(cmd.c_str())) {
        cerr << "ERROR @ Output: "
                << "System command failed to execute: " << cmd << endl;
    }
}

void
Output::printInfo() const {
    cout << " --- Output info --- " << endl;
    cout << " --- End of output info --- " << endl;
}

void Output::writeAllFields(const FieldD3& field) const {
    for (uint i = 0; i < field.size(); i++) {
        CVecD3 vec = field.getCVec(i);
        GiD_WriteVector(i, vec(0), vec(1), vec(2));
    }
}

string Output::getOutputfilename() const {
    string folder = getProjectFolder();
    string output = getOutputName();
    return folder + output;
}
