/*
 * Line.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#include "Line.h"

Line::Line() {

}

Line::~Line() {

}

Line::Line(const uint id_, const uint matId_, const uint layerId_) :
    Element(id_, matId_, layerId_) {
}
