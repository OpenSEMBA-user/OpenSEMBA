// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * Mesh.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */
#include "MeshStructured.h"
#include "MeshUnstructured.h"

MeshUnstructured::MeshUnstructured() {

}

MeshUnstructured::MeshUnstructured(const GroupCoordinates<const CoordR3>& cG,
                                   const GroupElements<const ElemR>& elem,
                                   const GroupLayers<const Layer>& layers)
:   GroupCoordinates<CoordR3>(cG.cloneElems()),
    GroupElements<ElemR>(elem.cloneElems()),
    GroupLayers<Layer>(layers.cloneElems()) {

    GroupElements<ElemR>::reassignPointers(*this);
}

MeshUnstructured::MeshUnstructured(const MeshUnstructured& rhs)
:   GroupCoordinates<CoordR3>(rhs.coords().cloneElems()),
    GroupElements<ElemR>(rhs.elems().cloneElems()),
    GroupLayers<Layer>(rhs.layers().cloneElems()) {

    GroupElements<ElemR>::reassignPointers(*this);
}

MeshUnstructured::~MeshUnstructured() {

}

MeshUnstructured& MeshUnstructured::operator=(const MeshUnstructured& rhs) {
    if(this == &rhs) {
        return *this;
    }

    GroupCoordinates<CoordR3>::operator=(rhs.coords().cloneElems());
    GroupElements<ElemR>::operator=(rhs.elems().cloneElems());
    GroupLayers<Layer>::operator=(rhs.layers().cloneElems());

    GroupElements<ElemR>::reassignPointers(*this);

    return *this;
}

MeshStructured* MeshUnstructured::getMeshStructured(const Grid3& grid,
        const Real tol) const {
    MeshStructured* res = new MeshStructured(grid);

    vector<CoordI3*> newCoords;
    newCoords.reserve(coords().size());
    for (UInt i = 0; i < coords().size(); i++) {
        CoordI3* newCoord = coords()(i)->toStructured(grid);
        if (newCoord != NULL) {
            newCoords.push_back(newCoord);
        }
    }
    res->coords().add(newCoords);

    vector<ElemI*> newElems;
    newElems.reserve(elems().size());
    for (UInt i = 0; i < elems().size(); i++) {
        ElemI* newElem = elems()(i)->toStructured(*res, grid, tol);
        if (newElem != NULL) {
            newElems.push_back(newElem);
        }
    }
    res->elems().add(newElems);
    res->layers() = layers().cloneElems();
    return res;
}

MeshUnstructured* MeshUnstructured::getConnectivityMesh() const {
    MeshUnstructured* res = new MeshUnstructured;
    res->coords() = coords().cloneElems();
    GroupElements<const ElemR> elems = this->elems();
    elems.removeMatId(MatId(0));
    GraphVertices<ElemR, CoordR3> graphLayer;
    graphLayer.init(elems, coords());
    vector<vector<const ElemR*>> comps = graphLayer.getConnectedComponents();
    for (UInt c = 0; c < comps.size(); c++) {
        stringstream layerName;
        layerName << "Component " << c+1;
        Layer* newLayer = res->layers().addId(new Layer(layerName.str()))(0);
        vector<ElemR*> newElemsLayer;
        newElemsLayer.resize(comps[c].size());
        for (UInt e = 0; e < comps[c].size(); e++) {
            newElemsLayer[e] = comps[c][e]->cloneTo<ElemR>();
            newElemsLayer[e]->setLayerId(newLayer->getId());
        }
        res->elems().add(newElemsLayer);
    }
    res->reassignPointers(*res);
    return res;
}

vector<Face> MeshUnstructured::getBorderWithNormal(const vector<Face>& border,
        const CVecR3& normal) {
    const UInt nK = border.size();
    vector<Face> res;
    res.reserve(nK);
    for (UInt i = 0; i < nK; i++) {
        const VolR* tet = border[i].first;
        const UInt face = border[i].second;
        CVecR3 tetNormal = tet->getSideNormal(face);
        if (tetNormal == normal && !tet->isCurvedFace(face)) {
            res.push_back(border[i]);
        }
    }
    return res;
}

GroupElements<const Triangle> MeshUnstructured::convertToTri(
        const GroupElements<const ElemR>& region,
        bool includeTets) const {

    GroupElements<Triangle> res(region.cloneElems());
    if (includeTets) {
        GroupElements<const Tetrahedron> tet = region.getOf<Tetrahedron>();
        vector<Face> border = getInternalBorder(tet);
        for (UInt i = 0; i < border.size(); i++) {
            if (border[i].first->is<Tetrahedron>()) {
                const Tetrahedron* tet = border[i].first->castTo<Tetrahedron>();
                const UInt face = border[i].second;
                res.addId(tet->getTri3Face(face));
            }
        }
    }
    return GroupElements<const Triangle>(res);
}

vector<Face> MeshUnstructured::getInternalBorder(
        const GroupElements<const ElemR>& region) const {
    Connectivities conn(region);
    vector<Face> res;
    res.reserve(region.size());
    for (UInt i = 0; i < region.size(); i++) {
        // TODO getInternalBorder
    }
    return res;
}

vector<Face> MeshUnstructured::getExternalBorder(
        const GroupElements<const ElemR>& region) const {

    vector<Face> internal = getInternalBorder(region);
    vector<Face> external;
    const Connectivities conn(region);
    external.reserve(internal.size());
    for (UInt i = 0; i < internal.size(); i++) {
        if (!conn.isDomainBoundary(internal[i]))  {
            external.push_back(conn.getNeighFace(internal[i]));
        }
    }
    return external;
}

GroupElements <const VolR> MeshUnstructured::getAdjacentRegion(
        const GroupElements<const VolR>& region) const {
    vector<Face> outer = getExternalBorder(region);
    UInt nOut = outer.size();
    // Removes repeated.
    DynMatrix<UInt> aux(nOut,1);
    for (UInt i = 0; i < nOut; i++) {
        aux(i,0) = outer[i].first->getId().toUInt();
    }
    aux.sortAndRemoveRepeatedRows_omp();
    // Prepares result.
    GroupElements<ElemR> res;
    for (UInt i = 0; i < aux.nRows(); i++) {
        res.add(elems().getId(ElementId(aux(i,0)))->cloneTo<ElemR>());
    }
    return res;
}

bool MeshUnstructured::isOnBoundary(const CVecR3 pos) const {
    // TODO Unstr isOnBoundary
}

GroupElements<const SurfR> MeshUnstructured::getMaterialBoundary(
        const MatId matId,
        const LayerId layId) const {

    return elems().getMatLayerId(matId, layId).getOf<SurfR>();
}

void MeshUnstructured::applyScalingFactor(const Real factor) {
    GroupCoordinates<CoordR3>::applyScalingFactor(factor);
}

void MeshUnstructured::printInfo() const {
    cout << " --- Mesh unstructured Info --- " << endl;
    cout << "Number of coordinates: " << GroupCoordinates<CoordR3>::size() << endl;
    cout << "Number of elements: " << GroupElements<ElemR>::size() << endl;
    GroupLayers<>::printInfo();
}

Real MeshUnstructured::getMinimumSpaceStep() const {
    // TODO Unstr minimum space step.
}

GroupElements<const SurfR> MeshUnstructured::getSurfsMatching(
        const vector<Face>& faces) const {
    vector<const SurfR*> res;
    IndexByVertexId index = getIndexByVertexId();
    for (UInt i = 0; i < faces.size(); i++) {
        const VolR* vol = faces[i].first;
        const UInt f = faces[i].second;
        vector<const CoordR3*> vertices = vol->getSideVertices(f);
        vector<CoordinateId> ids = ElementBase::getIds(vertices);
        IndexByVertexId::const_iterator it = index.find(ids);
        if (it != index.end()) {
            res.push_back(it->second->castTo<SurfR>());
        }
    }
    return GroupElements<const SurfR>(res);
}

BoxR3 MeshUnstructured::getBoundingBox() const {
    return elems().getBound();
}
