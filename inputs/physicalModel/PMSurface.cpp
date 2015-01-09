/*
 * PMSurface.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: luis
 */

#include "PMSurface.h"

PMSurface::PMSurface() {
}

PMSurface::PMSurface(const uint id, const string& name) :
   PhysicalModel(id, name) {

}

void PMSurface::printInfo() const {
   cout << "PM Surface:" << endl;
}
