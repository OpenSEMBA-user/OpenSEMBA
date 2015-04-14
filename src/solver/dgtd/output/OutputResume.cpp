/*
 * ResumeOutput.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: luis
 */

#include "OutputResume.h"

OutputResume::OutputResume() {
	// TODO Auto-generated constructor stub

}

OutputResume::~OutputResume() {
	// TODO Auto-generated destructor stub
}

//void
//ResumeOutput::writeResumeResFile(
// const double *Ex, const double *Ey, const double *Ez,
// const double *Hx, const double *Hy, const double *Hz,
// const int tStep, const uint numElems) {
//	// Initializes status flag.
//	int status = 0;
//	// Opens resuming .res file.
//	string auxStr = projectFolder + projectName + ".resume";
//	char *auxChar;
//	auxChar = new char[auxStr.length() + 1];
//	strcpy(auxChar, auxStr.c_str());
//	status += GiD_OpenPostResultFile(auxChar, GiD_PostAscii);
//	// Writes electric field results.
//	{
//		char * compv[] = {"Ex", "Ey", "Ez", "|E|"};
//		char * fName = "Electric Field (V/m)";
//		char * tName = "TimeStep";
//		status += GiD_BeginResult(fName, tName, tStep,
//					GiD_Vector, GiD_OnNodes, NULL, NULL, 4, compv);
//		status += writeAllFields(Ex, Ey, Ez, numElems);
//		status += GiD_EndResult();
//	}
//	// Writes magnetic field results.
//	{
//		char * compv[] = {"Hx", "Hy", "Hz", "|H|"};
//		char * fName = "Magnetic Field (A/m)";
//		char * tName = "TimeStep";
//		status += GiD_BeginResult(fName, tName, tStep,
//	    			GiD_Vector, GiD_OnNodes, NULL, NULL, 4, compv);
//		status += writeAllFields(Hx, Hy, Hz, numElems);
//		status += GiD_EndResult();
//	}
//	// Closes resuming .res file.
//	status += GiD_ClosePostResultFile();
//	cout << "File for resuming was succesfully written." << endl;
//	// Checks errors.
//	checkErrors(status);
//}
