/*
 * Quad.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#include "Quad.h"

Quad::Quad(const uint layerId,
           const uint matId)
:   Surface(layerId, matId) {

}

Quad::Quad(const ElementId id,
           const uint layerId,
           const uint matId)
:   Surface(id, layerId, matId) {

}

Quad::Quad(const Quad& rhs)
:   Surface(rhs) {

}

Quad::Quad(const ElementId id, const Quad& rhs)
:   Surface(id, rhs) {

}

Quad::~Quad() {

}
