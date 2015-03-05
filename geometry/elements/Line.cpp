/*
 * Line.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#include "Line.h"

Line::Line(const uint layerId,
           const uint matId)
:   Element(layerId, matId) {

}

Line::Line(const ElementId id,
           const uint layerId,
           const uint matId)
:   Element(id, layerId, matId) {

}

Line::Line(const Line& rhs)
:   Element(rhs) {

}

Line::Line(const ElementId id, const Line& rhs)
:   Element(id, rhs) {

}

Line::~Line() {

}
