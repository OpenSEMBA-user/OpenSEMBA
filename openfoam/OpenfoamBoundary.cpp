/*
 * OpenfoamBoundary.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: luis
 */

#include "OpenfoamBoundary.h"

OpenfoamBoundary::OpenfoamBoundary() {
    nFaces_ = 0;
    startFace_ = 0;
}

OpenfoamBoundary::OpenfoamBoundary(
        const string name,
        const UInt nFaces,
        const UInt startFace) : Layer(LayerId(0), name) {
    nFaces_ = nFaces;
    startFace_ = startFace;
    if (isMaterial()) {
        setId(getLayerIdFromName());
    }
}

UInt
OpenfoamBoundary::getFaces() const {
    return nFaces_;
}

UInt
OpenfoamBoundary::getStartFace() const {
    return startFace_;
}

void
OpenfoamBoundary::printInfo() const {
    cout << " --- OpenFoam boundary info ---" << endl;
    Layer::printInfo();
    cout << "Nfaces: " << nFaces_ << endl;
    cout << "StartFace: " << startFace_ << endl;
}

bool
OpenfoamBoundary::isMaterial() const {
    if (getName().find("mat.") != getName().npos) {
        return true;
    }
    return false;
}

MatId
OpenfoamBoundary::getMaterialIdFromName() const {
    assert(isMaterial());
    Int firstDot = strpos(getName().c_str(), ".", 1);
    Int secondDot = strpos(getName().c_str(), ".", 2);
    string values = getName().substr(firstDot + 1, secondDot);
    UInt res = atoi(values.c_str());
    return MatId(res);
}

OpenfoamBoundary::~OpenfoamBoundary() {
}

LayerId
OpenfoamBoundary::getLayerIdFromName() const {
    assert(isMaterial());
    Int secondDot = strpos(getName().c_str(), ".", 2);
    string value = getName().substr(secondDot + 1, getName().npos);
    UInt res = atoi(value.c_str());
    return LayerId(res);
}

Int
OpenfoamBoundary::strpos(const char *haystack,
                         const char *needle, Int nth) const {
    const char *res = haystack;
    for(Int i = 1; i <= nth; i++)
    {
        res = strstr(res, needle);
        if (!res) {
            return -1;
        } else if (i != nth) {
            res++;
        }
    }
    return res - haystack;
}
