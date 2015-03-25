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
                               const CVecR3& norm,
                               const Int label,
                               const LayerId layerId,
                               const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId),
    LinI2(v) {

    checkCoordinates();

    norm_  = norm;
    label_ = label;
}

LineConformal::LineConformal(const CoordI3* v[2],
                             const CVecR3& norm,
                             const Int label) {
    checkCoordinates();

    norm_  = norm;
    label_ = label;
}

LineConformal::LineConformal(const LineConformal& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs),
    LinI2(rhs) {

    norm_  = rhs.norm_;
    label_ = rhs.label_;
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

ElemR* LineConformal::toUnstructured(CoordinateGroup<CoordR3>& cG,
                                     const Grid3& grid) const {
    const CoordConf* vConf[2];
    vConf[0] = getConfV(0);
    vConf[1] = getConfV(1);

    CVecR3 pos;
    const CoordR3* coord[2];
    CoordinateId coordId;
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        pos = grid.getPos(vConf[i]->pos());
        if (MathUtils::greather(vConf[i]->getLength(), 0.0, 1.0)) {
            Int dir = vConf[i]->getDir();
            Real length = vConf[i]->getLength();
            CVecI3 cellAux = vConf[i]->pos();
            cellAux(dir)++;
            CVecR3 posAux = grid.getPos(cellAux);
            Real step = posAux(dir)-pos(dir);
            pos(dir) += step*length;
        }
        coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            cG.add(new CoordR3(coordId, pos));
        }
        coord[i] = cG.getPtrToId(coordId);
        if (coord[i]->pos() != pos) {
            cerr << endl << "ERROR @ LineConformal::toUnstructured(): "
                 << "Existent Coordinate not coincident." << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
    }
    return new LinR2(this->getId(),
                     coord,
                     this->getLayerId(),
                     this->getMatId());
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
