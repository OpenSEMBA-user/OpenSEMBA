/*
 * MagnitudeNumerical.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#include "MagnitudeNumerical.h"

MagnitudeNumerical::MagnitudeNumerical() {
   // TODO Auto-generated constructor stub

}

MagnitudeNumerical::MagnitudeNumerical(const string& filename)
   : ProjectFile(filename) {
   initFromFile();
}

MagnitudeNumerical::MagnitudeNumerical(
      const string& name,
      const Magnitude* mag,
      const Real timeStep,
      const Real finalTime) :
         ProjectFile(name) {
   const MagnitudeNumerical* magNum
    = dynamic_cast<const MagnitudeNumerical*>(mag);
   if (magNum != NULL) {
      *this = *magNum;
      initFromFile();
      return;
   }
   UInt nSteps;
   if (timeStep != 0.0) {
      nSteps = abs(finalTime / timeStep);
   } else {
      nSteps = defaultNumberOfSteps;
      cerr << endl << "WARNING @ MagnitudeNumerical: "
            << "Attempting to build a numerical magnitude with a 0.0 step."
            << "Using default number of steps instead: " << nSteps << endl;
      mag->printInfo();
   }
   ofstream file;
   file.open(name.c_str());
   Real time = 0.0;
   for (UInt i = 0; i < nSteps; i++) {
      file << time << " " << mag->evaluate(time) << endl;
      time += timeStep;
   }
   file.close();
   initFromFile();
}

MagnitudeNumerical::~MagnitudeNumerical() {
   // TODO Auto-generated destructor stub
}

void MagnitudeNumerical::initFromFile() {
   ifstream file;
   file.open(getFilename().c_str(), ifstream::in);
   if (file.fail()) {
      cerr << endl << "ERROR @ MagnitudeNumerical ctor: " << "Problem opening file: "
            << getFilename() << endl;
      printInfo();
   }
   while (!file.eof()) {
      pair<Real, Real> value;
      file >> value.first >> value.second;
      value_.insert(value);
   }
   if (value_.size() == 0) {
      cerr << endl << "WARNING @ MagnitudeNumerical: "
            << "No values where readed from file: " << getFilename() << endl;
   }
}

void
MagnitudeNumerical::printInfo() const {
   cout << " --- Magnitude Numerical Info --- " << endl;
   ProjectFile::printInfo();
   cout << "Stored values: " << value_.size() << endl;
   //    map<Real,Real>::const_iterator it;
   //    for (it = value_.begin(); it != value_.end(); ++it) {
   //        cout << it->first << " " << it->second << endl;
   //    }
}

Real
MagnitudeNumerical::evaluate(const Real time) const {
   cerr << endl << "ERROR @ MagnitudeNumerical: "
         << "Evaluate not implemented." << endl;
   assert(false);
   return 0.0;
}

MagnitudeNumerical&
MagnitudeNumerical::operator =(
      const MagnitudeNumerical& rhs) {
   if (this == &rhs) {
      return *this;
   }
   value_ = rhs.value_;
   Magnitude::operator=(rhs);
   ProjectFile::operator=(rhs);
   return *this;
}
