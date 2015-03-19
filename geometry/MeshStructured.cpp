/*
 * MeshStructured.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#include "MeshStructured.h"

MeshStructured::MeshStructured() {

}

MeshStructured::MeshStructured(const Grid3& grid,
                               const CoordinateGroup<CoordI3>& cG,
                               const ElementsGroup<ElemI>& elem,
                               const LayerGroup<>& layers)
:   Mesh(layers),
    Grid3(grid),
    CoordinateGroup<CoordI3>(cG.newGroupOf<CoordI3>()),
    ElementsGroup<ElemI>(elem.newGroupOf<ElemI>()) {

    ElementsGroup<ElemI>::reassignPointers(*this);
}

MeshStructured::MeshStructured(const MeshStructured& rhs)
:   Mesh(rhs),
    Grid3(rhs),
    CoordinateGroup<CoordI3>(rhs.coords().newGroupOf<CoordI3>()),
    ElementsGroup<ElemI>(rhs.elems().newGroupOf<ElemI>()) {

    ElementsGroup<ElemI>::reassignPointers(*this);
}

MeshStructured::~MeshStructured() {

}

MeshStructured& MeshStructured::operator=(const MeshStructured& rhs) {
    if(this == &rhs) {
        return *this;
    }

    Mesh::operator=(rhs);
    Grid3::operator=(rhs);
    CoordinateGroup<CoordI3>::operator=(rhs.coords().newGroupOf<CoordI3>());
    ElementsGroup<ElemI>::operator=(rhs.elems().newGroupOf<ElemI>());

    ElementsGroup<ElemI>::reassignPointers(*this);

    return *this;
}

ClassBase* MeshStructured::clone() const {
    return new MeshStructured(*this);
}

vector<BoxR3>
MeshStructured::discretizeWithinBoundary(
        const UInt matId, const UInt layId) const {
#warning "Not implemented"
}

void MeshStructured::applyScalingFactor(const Real factor) {
    Grid3::applyScalingFactor(factor);
}

void MeshStructured::printInfo() const {
    cout << " --- Mesh structured info --- " << endl;
    Grid3::printInfo();
    CoordinateGroup<CoordI3>::printInfo();
    ElementsGroup<ElemI>::printInfo();
    Mesh::printInfo();
}
