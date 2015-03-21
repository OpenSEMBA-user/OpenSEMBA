/*
 * Lin2Conformal.cpp
 *
 *  Created on: 21/3/2015
 *      Author: Daniel
 */

#include "LineConformal.h"

LineConformal::LineConformal() {
    checkCoordinates();
}

LineConformal::LineConformal(const ElementId id,
                               const CoordI3* v[2],
                               const LayerId layerId,
                               const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId),
    LinI2(v) {

    checkCoordinates();
}

LineConformal::LineConformal(const CoordI3* v[2]) {
    checkCoordinates();
}

LineConformal::LineConformal(const LineConformal& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs),
    LinI2(rhs) {

}

LineConformal::~LineConformal() {

}

const CoordConf* LineConformal::getConfV(const UInt i) const {
    return this->getV(i)->castTo<CoordConf>();
}

ClassBase* LineConformal::clone() const {
    return new LineConformal(*this);
}

void LineConformal::setV(const UInt i, const CoordI3* coord) {
    if (!coord->is<CoordConf>()) {
        cerr << endl << "ERROR @ Lin2Conformal::setV(): "
             << "Invalid Coordinate with Id: " << this->getV(i)->getId()
             << endl;
        assert(false);
        exit(EXIT_FAILURE);
    }
    LinI2::setV(i, coord);
}

void LineConformal::printInfo() const {
    cout << "--- LinI2Conformal info ---" << endl;
    cout << "Id: " << this->getId() << endl;
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        getV(i)->printInfo();
        cout << endl;
    }
}

void LineConformal::checkCoordinates() {
    for(UInt i = 0; i < this->numberOfCoordinates(); i++) {
        if (!this->getV(i)->is<CoordConf>()) {
            cerr << endl << "ERROR @ LineConformal::checkCoordinates(): "
                 << "Invalid Coordinate with Id: " << this->getV(i)->getId()
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
    }
}
