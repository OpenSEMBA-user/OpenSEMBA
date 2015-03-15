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
vector<pair<const E*, UInt> > ElementsGroup<E>::getElementsWithVertex(
        const CoordinateId vertexId) const {

    vector<pair<const E*, UInt> > res;
    ElementsGroup<ElemR> elemsR = this->template getGroupOf<ElemR>();
    for (UInt i = 0; i < elemsR.size(); i++) {
        for (UInt j = 0; j < elemsR(i)->numberOfVertices(); j++) {
            if ((elemsR(i)->getVertex(j)->getId() == vertexId) &&
                elemsR(i)->template is<E>()) {

                pair<const E*, UInt> aux(elemsR(i)->template castTo<E>(), j);
                res.push_back(aux);
            }
        }
    }
    ElementsGroup<ElemI> elemsI = this->template getGroupOf<ElemI>();
    for (UInt i = 0; i < elemsI.size(); i++) {
        for (UInt j = 0; j < elemsI(i)->numberOfVertices(); j++) {
            if ((elemsI(i)->getVertex(j)->getId() == vertexId) &&
                elemsI(i)->template is<E>()) {

                pair<const E*, UInt> aux(elemsI(i)->template castTo<E>(), j);
                res.push_back(aux);
            }
        }
    }
    return res;
}

template<typename E>
bool ElementsGroup<E>::isLinear() const {
    return (this->template sizeOf<Tri6 >() == 0 &&
            this->template sizeOf<Tet10>() == 0);
}

template<typename E>
void ElementsGroup<E>::setMaterialIds(const vector<ElementId>& id,
                                      const MatId newMatId) {
    for (UInt i = 0; i < id.size(); i++) {
        this->elements_[this->mapId_[id[i]]]->setMatId(newMatId);
    }
}

template<typename E>
BoxR3 ElementsGroup<E>::getBound() const {
    if (this->size() == 0) {
        return BoxR3().setInfinity();
    }
    BoxR3 bound;
    ElementsGroup<ElemR> elems = this->template getGroupOf<ElemR>();
    for (UInt i = 0; i < elems.size(); i++) {
        bound << elems(i)->getBound();
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
ElementsGroup<E> ElementsGroup<E>::get(const vector<ElementId>& ids) const {
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
const CoordR3* ElementsGroup<E>::getClosestVertex(const CVecR3 pos) const {
    const CoordR3* res;
    Real minDist = numeric_limits<Real>::infinity();
    ElementsGroup<ElemR> elems = this->template getGroupOf<ElemR>();
    for (UInt b = 0; b < elems.size(); b++) {
        for (UInt i = 0; i < elems(i)->numberOfCoordinates(); i++) {
            const CoordR3* candidate = elems(i)->getV(i);
            if ((candidate->pos() - res->pos()).norm() < minDist) {
                res = candidate;
                minDist = (candidate->pos() - res->pos()).norm();
            }
        }
    }
    return res;
}

template<typename E>
vector<ElementId> ElementsGroup<E>::getIdsInsideBound(
        const BoxR3& bound) const {

    ElementsGroup<ElemR> elems = this->template getGroupOf<ElemR>();
    vector<ElementId> res;
    res.reserve(elems.size());
    for (UInt i = 0; i < elems.size(); i++) {
        BoxR3 localBound = elems(i)->getBound();
        if (localBound <= bound) {
            res.push_back(elems(i)->getId());
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
    return ElementsGroup<E>(elems);
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
        } else if (this->element_[i]->template is<ElemI>()) {
            ElemI* elem = this->element_[i]->template castTo<ElemI>();
            for (UInt j = 0; j < elem->numberOfCoordinates(); j++) {
                CoordinateId vId = elem->getV(j)->getId();
                const CoordinateBase* coord = vNew.getPtrToId(vId);
                if (coord == NULL) {
                    cerr << endl << "ERROR @ ElementsGroup<E>: "
                         << "Coordinate in new CoordinateGroup inexistent"
                         << endl;
                }
                if (!coord->is<CoordI3>()) {
                    cerr << endl << "ERROR @ ElementsGroup<E>: "
                         << "Coord in new CoordinateGroup is not a valid Coord"
                         << endl;
                }
                elem->setV(j, coord->castTo<CoordI3>());
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
