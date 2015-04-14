/*
 * OpenfoamBoundary.h
 *
 *  Created on: Apr 11, 2014
 *      Author: luis
 */

#ifndef OPENFOAMBOUNDARY_H_
#define OPENFOAMBOUNDARY_H_

#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <assert.h>

using namespace std;

#include "../../../common/geometry/layers/Layer.h"
#include "PhysicalModel.h"
#include "Types.h"

class OpenfoamBoundary : public Layer {
    friend class ParserOpenFoam;
public:
	OpenfoamBoundary();
	OpenfoamBoundary(
	 const string name,
	 const UInt nFaces,
	 const UInt startFace);
	virtual ~OpenfoamBoundary();
	UInt getFaces() const;
	UInt getStartFace() const;
    void printInfo() const;
protected:
    bool isMaterial() const;
    MatId getMaterialIdFromName() const;
private:
	UInt nFaces_;
	UInt startFace_;
	Int strpos(const char *haystack, char *needle, Int nth) const;
    LayerId getLayerIdFromName() const;
};

#endif /* OPENFOAMBOUNDARY_H_ */
