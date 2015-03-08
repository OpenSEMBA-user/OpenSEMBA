/*
 * Line.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#include "Line.h"

Line::Line(const LayerId layerId,
           const uint matId)
:   Element(layerId, matId) {

}

Line::Line(const ElementId id,
           const LayerId layerId,
           const uint matId)
:   Element(id, layerId, matId) {

}

Line::Line(const Line& rhs)
:   Element(rhs) {

}

Line::~Line() {

}
