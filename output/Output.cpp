#ifndef OUTPUT_H_
#include "Output.h"
#endif

Output::Output()  {
   smb_ = NULL;
   dg_ = NULL;
}

Output::Output(
      const SmbData* smb,
      const DG* dg) : ProjectFile(smb->getFilename()) {
   smb_ = smb;
   dg_ = dg;
//   result_.push_back(new ResultRCS(getOutputfilename() + ".post.rcs", smb_));
}

Output::~Output() {
   for (uint i = 0; i < result_.size(); i++) {
      delete result_[i];
   }
}

void
Output::writeResumeFile(
      const double time, const FieldD3& electric, const FieldD3& magnetic) {
   string auxStr = smb_->getProjectFolder()+smb_->getProjectName()+".resume";
   char *auxChar;
   auxChar = new char[auxStr.length() + 1];
   strcpy(auxChar, auxStr.c_str());
   GiD_OpenPostResultFile(auxChar, GiD_PostAscii);
   {
      const char * compv[] = {"Ex", "Ey", "Ez", "|E|"};
      const char * fName = "Electric Field";
      const char * tName = "Time";
      GiD_BeginResult((char*)fName, (char*)tName, time,
            GiD_Vector, GiD_OnNodes, NULL, NULL, 4, (char**)compv);
      writeAllFields(electric);
      GiD_EndResult();
   }
   {
      const char * compv[] = {"Hx", "Hy", "Hz", "|H|"};
      const char * fName = "Magnetic Field";
      const char * tName = "Time";
      GiD_BeginResult((char*)fName, (char*)tName, time,
            GiD_Vector, GiD_OnNodes, NULL, NULL, 4, (char**)compv);
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
   string fileName = smb_->getProjectFolder() + smb_->getProjectName();
   string cmd;
   cmd = "rm -f " + fileName + ".post.msh";
   system(cmd.c_str());
   cmd = "rm -f " + fileName + ".post.res";
   system(cmd.c_str());
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
   return smb_->getProjectFolder() + smb_->getOutputName();
}
