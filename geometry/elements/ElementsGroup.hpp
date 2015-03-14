/*
 * ElementsGroup.hpp
 *
 *  Created on: Mar 28, 2013
 *      Author: luis
 */

#include "ElementsGroup.h"

template<typename E>
ElementsGroup<E>::ElementsGroup() {

}

template<typename E>
ElementsGroup<E>::ElementsGroup(const vector<E*>& elems, bool ownership)
:   GroupId<E, ElementId>(elems, ownership) {

}

template<typename E>
ElementsGroup<E>::ElementsGroup(const Group<E>& rhs)
:   GroupId<E, ElementId>(rhs) {

}

template<typename E>
ElementsGroup<E>::~ElementsGroup() {

}

template<typename E>
ElementsGroup<E>& ElementsGroup<E>::operator=(const Group<E>& rhs) {
    if (this == &rhs) {
        return *this;
    }

    GroupId<E, ElementId>::operator=(rhs);

    return *this;
}

template<typename E>
vector<pair<const ElementBase*, UInt> >
ElementsGroup<E>::getElementsWithVertex(const CoordinateId vertexId) const {
    vector<pair<const ElementBase*, UInt> > res;
    for (UInt i = 0; i < this->size(); i++) {
        for (UInt j = 0; j < this->element_[i]->numberOfVertices(); j++) {
            if (this->element_[i]->getVertex(j)->getId() == vertexId) {
                pair<const ElemR*, UInt> aux(this->element_[i], j);
                res.push_back(aux);
            }
        }
    }
    return res;
}

template<typename E>
bool ElementsGroup<E>::isLinear() const {
    return (this->template sizeOf<Tri6>() == 0 &&
            this->template sizeOf<Tet10>() == 0);
}

template<typename E>
void ElementsGroup<E>::setMaterialIds(
 const vector<ElementId>& id,
 const MatId newMatId) {
    for (UInt i = 0; i < id.size(); i++) {
        this->getPtrToId(id[i])->setMatId(newMatId);
    }
}

template<typename E>
BoxR3 ElementsGroup<E>::getBound() const {
    if (this->size() == 0) {
        return BoxR3().setInfinity();
    }
    BoxR3 bound = this->element_[0]->getBound();
    for (UInt i = 1; i < this->size(); i++) {
        bound << this->element_[i]->getBound();
    }
    return bound;
}

template<typename E>
BoxR3 ElementsGroup<E>::getBound(const vector<Face>& border) const {
    if (border.size() == 0) {
        return BoxR3().setInfinity();
    }
    const VolR* tet = border[0].first;
    const UInt face = border[0].second;
    BoxR3 bound = tet->getBoundOfFace(face);
    for (UInt i = 1; i < border.size(); i++) {
        const VolR* vol = border[i].first;
        const UInt face = border[i].second;
        bound << vol->getBoundOfFace(face);
    }
    return bound;
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::get(
    const vector<ElementId>& ids) const {

    return GroupId<E, ElementId>::get(ids);
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::get(const MatId matId) const {
    vector<MatId> aux;
    aux.push_back(matId);
    return get(aux);
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::get(const vector<MatId>& matIds_) const {
    set<MatId> matIds(matIds_.begin(), matIds_.end());
    vector<E*> res;
    res.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        if (matIds.count(this->element_[i]->getMatId()) == 1) {
            res.push_back(this->element_[i]);
        }
    }
    return ElementsGroup<E>(res, false);
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::get(const LayerId layerId) const {
    vector<LayerId> aux;
    aux.push_back(layerId);
    return get(aux);
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::get(const vector<LayerId>& layIds_) const {
    set<LayerId> layIds(layIds_.begin(), layIds_.end());
    vector<E*> res;
    res.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        if (layIds.count(this->element_[i]->getLayerId()) == 1) {
            res.push_back(this->element_[i]);
        }
    }
    return ElementsGroup<E>(res, false);
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::get(const MatId   matId,
                                       const LayerId layId) const {
    return get(matId).get(layId);
}

template<typename E>
const CoordR3*
ElementsGroup<E>::getClosestVertex(const CVecR3 pos) const {
    const CoordR3* res;
    Real minDist = numeric_limits<Real>::infinity();
    for (UInt b = 0; b < this->size(); b++) {
        for (UInt i = 0; i < this->element_[b]->numberOfCoordinates(); i++) {
            const CoordR3* candidate = this->element_[b]->getV(i);
            if ((candidate->pos() - res->pos()).norm() < minDist) {
                res = candidate;
            }
        }
    }
    return res;
}

template<typename E>
vector<ElementId>
ElementsGroup<E>::getIdsInsideBound(const BoxR3& bound) const {
    const UInt nK = this->size();
    vector<ElementId> res;
    res.reserve(nK);
    for (UInt i = 0; i < nK; i++) {
        const ElementBase* e = this->element_[i];
        BoxR3 localBound = e->getBound();
        if (localBound <= bound) {
            res.push_back(e->getId());
        }
    }
    return res;
}

template<typename E>
vector<ElementId> ElementsGroup<E>::getIdsWithoutMaterialId(
    const MatId matId) const {

    vector<ElementId> res;
    res.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        if (this->element_[i]->getMatId() != matId) {
            res.push_back(this->element_[i]->getId());
        }
    }
    return res;
}

template<typename E>
void ElementsGroup<E>::add(E* newElem, bool newId) {
    return GroupId<E, ElementId>::add(newElem, newId);
}

template<typename E>
void ElementsGroup<E>::add(vector<E*>& newElems, bool newId) {
    return GroupId<E, ElementId>::add(newElems, newId);
}

template<typename E>
vector<ElementId> ElementsGroup<E>::add(const CoordinateGroup<>& coord,
                                        const vector<HexR8>& newHex) {
    CoordinateId vId[8];
    MatId matId;
    vector<ElementId> res;
    for (UInt i = 0; i < newHex.size(); i++) {
        // Determines coordinates ids.
        // PERFORMANCE This is O(N^2). It can be improved by creating a
        //               lexicographically sorted list of coordinates positions.
        for (UInt j = 0; j < 8; j++) {
            vId[j] = newHex[i].getV(j)->getId();
        }
        matId = newHex[i].getMatId();
        HexR8* newhex =
            new HexR8(coord, ElementId(0), vId, LayerId(0), matId);
        if(newhex->is<E>()) {
            this->add(newhex->template castTo<E>(), true);
            res.push_back(newhex->getId());
        } else {
            delete newhex;
        }
    }
    return res;
}

template<typename E>
map<LayerId, ElementsGroup<E> > ElementsGroup<E>::separateByLayers() const {
    map<LayerId, ElementsGroup<E> > res;
    for (UInt i = 0; i < this->size(); i++) {
        const LayerId layerId = this->element_[i]->getLayerId();
        typename map<LayerId, ElementsGroup<E> >::iterator it =
            res.find(layerId);
        if (it == res.end()) {
            pair<LayerId, ElementsGroup<E> > newEntry;
            newEntry.first = layerId;
            newEntry.second.add(this->element_[i]);
            res.insert(newEntry);
        } else {
            it->second.add(this->element_[i]);
        }
    }
    return res;
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::removeElementsWithMatId(
    const MatId matId) const {

    vector<E*> elems;
    elems.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        if (this->element_[i]->getMatId() != matId) {
            elems.push_back(this->element_[i]->clone()->template castTo<E>());
        }
    }
    return ElementsGroup<E>(elems, false);
}

template<typename E>
void ElementsGroup<E>::reassignPointers(const CoordinateGroup<>& vNew) {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->element_[i]->template is<ElemR>()) {
            ElemR* elem = this->element_[i]->template castTo<ElemR>();
            for (UInt j = 0; j < elem->numberOfCoordinates(); j++) {
                CoordinateId vId = elem->getV(j)->getId();
                const CoordinateBase* coord = vNew.getPtrToId(vId);
                if (coord == NULL) {
                    cerr << endl << "ERROR @ ElementsGroup<E>: "
                         << "Coordinate in new CoordinateGroup inexistent"
                         << endl;
                }
                if (!coord->is<CoordR3>()) {
                    cerr << endl << "ERROR @ ElementsGroup<E>: "
                         << "Coord in new CoordinateGroup is not a valid Coord"
                         << endl;
                }
                elem->setV(j, coord->castTo<CoordR3>());
            }
        }
    }
}

template<typename E>
void ElementsGroup<E>::linearize() {
    if (isLinear()) {
        return;
    }
    if(!this->ownership_) {
        cerr << endl << "ERROR @ ElementsGroup::linearize(): "
             << "Forbidden to linearize without ownership "
             << "of elements on it" << endl;
        assert(false);
        exit(EXIT_FAILURE);
    }
    assert(this->template sizeOf<Lin2>() == 0);
    assert(this->template sizeOf<Tri3>() == 0);
    assert(this->template sizeOf<Tet4>() == 0);

    for(UInt i = 0; i < this->size(); i++) {
        if (this->element_[i]->template is<Tri6> ()) {
            Tri6* tmpPtr = this->element_[i]->template castTo<Tri6>();
            this->element_[i] = tmpPtr->linearize();
            delete tmpPtr;
        }
        if (this->element_[i]->template is<Tet10> ()) {
            Tet10* tmpPtr = this->element_[i]->template castTo<Tet10>();
            this->element_[i] = tmpPtr->linearize();
            delete tmpPtr;
        }
    }
}

template<typename E>
void ElementsGroup<E>::printInfo() const {
    cout << "--- Elements Group info ---" << endl;
    cout << "Total number of elements: " << this->size() << endl;
    for(UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
        cout << endl;
    }
}
