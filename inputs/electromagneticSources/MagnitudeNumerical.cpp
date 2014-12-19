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

MagnitudeNumerical::MagnitudeNumerical(const string& filename) {
   initFromFile(filename);
}

MagnitudeNumerical::MagnitudeNumerical(
      const Magnitude* mag,
      const double timeStep,
      const double finalTime) {
   const MagnitudeNumerical* magNum
    = dynamic_cast<const MagnitudeNumerical*>(mag);
   if (magNum != NULL) {
      initFromFile(magNum->getFilename());
      return;
   }
   const string name("predefinedExcitation.dat");
   unsigned int nSteps;
   if (timeStep != 0.0) {
      nSteps = abs(finalTime / timeStep);
   } else {
      nSteps = defaultNumberOfSteps;
      cerr<< "WARNING @ MagnitudeNumerical: "
            << "Attempting to build a numerical magnitude with a 0.0 step."
            << "Using default number of steps instead: " << nSteps << endl;
      mag->printInfo();
   }
   ofstream file;
   file.open(name.c_str());
   double time = 0.0;
   for (unsigned int i = 0; i < nSteps; i++) {
      file << time << mag->evaluate(time) << endl;
   }
   file.close();
   initFromFile(name);
}

MagnitudeNumerical::~MagnitudeNumerical() {
   // TODO Auto-generated destructor stub
}

const string& MagnitudeNumerical::getFilename() const {
   return filename_;
}

void MagnitudeNumerical::initFromFile(const string& filename) {
   filename_ = filename;
   ifstream file;
   file.open(filename_.c_str(), ifstream::in);
   if (file.fail()) {
      cerr << "ERROR @ MagnitudeNumerical ctor: " << "Problem opening file: "
            << file << endl;
   }
   while (!file.eof()) {
      pair<double, double> value;
      file >> value.first >> value.second;
      value_.insert(value);
   }
   if (value_.size() == 0) {
      cerr << "WARNING @ MagnitudeNumerical: "
            << "No values where readed from file: " << filename_ << endl;
   }
}

void
MagnitudeNumerical::printInfo() const {
   cout << " --- Magnitude Numerical Info --- " << endl;
   cout << "File name: " << filename_ << endl;
   cout << "Stored values: " << value_.size() << endl;
   //    map<double,double>::const_iterator it;
   //    for (it = value_.begin(); it != value_.end(); ++it) {
   //        cout << it->first << " " << it->second << endl;
   //    }
}

double
MagnitudeNumerical::evaluate(const double time) const {
   cerr<< "ERROR @ MagnitudeNumerical: "
         << "Evaluate not implemented." << endl;
   assert(false);
   return 0.0;
}
