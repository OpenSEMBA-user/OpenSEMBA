/*
 * ElementsGroup.hpp
 *
 *  Created on: Mar 28, 2013
 *      Author: luis
 */

#include "ElementsGroup.h"

template<typename E>
bool ElementsGroup<E>::isLinear() const {
    return (this->template sizeOf<Tri6 >() == 0 &&
            this->template sizeOf<Tet10>() == 0);
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::getGroupWith(const MatId matId) {
    vector<MatId> aux;
    aux.push_back(matId);
    return getGroupWith(aux);
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::getGroupWith(
        const vector<MatId>& matIds) {

    return this->getGroupWith(getElemsWith_(matIds));
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::getGroupWith(const LayerId layerId) {
    vector<LayerId> aux;
    aux.push_back(layerId);
    return getGroupWith(aux);
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::getGroupWith(
        const vector<LayerId>& layIds) {

    return this->getGroupWith(getElemsWith_(layIds));
}

template<typename E>
ElementsGroup<E> ElementsGroup<E>::getGroupWith(const MatId   matId,
                                                const LayerId layId) {
    return getGroupWith(matId).getGroupWith(layId);
}

template<typename E>
ElementsGroup<const E> ElementsGroup<E>::getGroupWith(
        const MatId matId) const {
    vector<MatId> aux;
    aux.push_back(matId);
    return getGroupWith(aux);
}

template<typename E>
ElementsGroup<const E> ElementsGroup<E>::getGroupWith(
        const vector<MatId>& matIds) const {

    return this->getGroupWith(getElemsWith_(matIds));
}

template<typename E>
ElementsGroup<const E> ElementsGroup<E>::getGroupWith(
        const LayerId layerId) const {
    vector<LayerId> aux;
    aux.push_back(layerId);
    return getGroupWith(aux);
}

template<typename E>
ElementsGroup<const E> ElementsGroup<E>::getGroupWith(
        const vector<LayerId>& layIds) const {

    return this->getGroupWith(getElemsWith_(layIds));
}

template<typename E>
ElementsGroup<const E> ElementsGroup<E>::getGroupWith(
        const MatId   matId,
        const LayerId layId) const {
    return getGroupWith(matId).getGroupWith(layId);
}

template<typename E>
vector<ElementId> ElementsGroup<E>::getIdsWithoutMaterialId(
        const MatId matId) const {

    vector<ElementId> res;
    res.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->getMatId() != matId) {
            res.push_back(this->get(i)->getId());
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
BoxR3 ElementsGroup<E>::getBound() const {
    if (this->size() == 0) {
        return BoxR3().setInfinity();
    }
    BoxR3 bound;
    ElementsGroup<const ElemR> elems = this->template getGroupOf<ElemR>();
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
    BoxR3 bound;
    for (UInt i = 0; i < border.size(); i++) {
        const VolR* vol = border[i].first;
        const UInt face = border[i].second;
        bound << vol->getBoundOfFace(face);
    }
    return bound;
}

template<typename E>
const CoordR3* ElementsGroup<E>::getClosestVertex(const CVecR3 pos) const {
    const CoordR3* res = NULL;
    Real minDist = numeric_limits<Real>::infinity();
    ElementsGroup<const ElemR> elems = this->template getGroupOf<ElemR>();
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
void ElementsGroup<E>::setMatId(const MatId newId) {
    for (UInt i = 0; i < this->size(); i++) {
        this->get(i)->setMatId(newId);
    }
}

template<typename E>
void ElementsGroup<E>::setLayerId(const LayerId newId) {
    for (UInt i = 0; i < this->size(); i++) {
        this->get(i)->setLayerId(newId);
    }
}

template<typename E>
void ElementsGroup<E>::setMatId(const ElementId id,
                                const MatId newMatId) {
    this->get(id)->setMatId(newMatId);
}

template<typename E>
void ElementsGroup<E>::setLayerId(const ElementId id,
                                  const LayerId newId) {
    this->get(id)->setLayerId(newId);
}

template<typename E>
void ElementsGroup<E>::remove(const MatId matId) {
    vector<MatId> aux;
    aux.push_back(matId);
    remove(aux);
}

template<typename E>
void ElementsGroup<E>::remove(const vector<MatId>& matId) {
    Group<E>::remove(getElemsWith_(matId));
}

template<typename E> template<class T>
void ElementsGroup<E>::reassignPointers(
        const CoordinateGroup< Coordinate<T,3> >& vNew) {

    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->template is< Element<T> >()) {
            Element<T>* elem = this->get(i)->template castTo< Element<T> >();
            for (UInt j = 0; j < elem->numberOfCoordinates(); j++) {
                CoordinateId vId = elem->getV(j)->getId();
                elem->setV(j, vNew.get(vId));
            }
        }
    }
}

template<typename E>
map<LayerId, vector<const E*> >
ElementsGroup<E>::separateByLayers() const {
    map<LayerId, vector<const E*> > res;
    for (UInt i = 0; i < this->size(); i++) {
        const LayerId layerId = this->get(i)->getLayerId();
        typename map<LayerId, vector<const E*> >::iterator it =
            res.find(layerId);
        if (it == res.end()) {
            pair<LayerId, vector<const E*> > newEntry;
            newEntry.first = layerId;
            newEntry.second.push_back(this->get(i));
            res.insert(newEntry);
        } else {
            it->second.push_back(this->get(i));
        }
    }
    return res;
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

    vector<UInt> pos;
    vector<Tri3*> newTri;
    vector<Tet4*> newTet;
    for(UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->template is<Tri6>()) {
            pos.push_back(i);
            newTri.push_back(this->get(i)->linearize());
        }
        if (this->element_[i]->template is<Tet10>()) {
            pos.push_back(i);
            newTet.push_back(this->get(i)->linearize());
        }
    }
    this->remove(pos);
    this->add(newTri);
    this->add(newTet);
}

template<typename E>
void ElementsGroup<E>::printInfo() const {
    cout << "--- Elements Group info ---" << endl;
    Group<E>::printInfo();
}

template<typename E>
vector<UInt> ElementsGroup<E>::getElemsWith_(
        const vector<MatId>& mats) const {

    set<MatId> matIds(mats.begin(), mats.end());
    vector<UInt> elems;
    elems.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        if (matIds.count(this->get(i)->getMatId()) == 1) {
            elems.push_back(i);
        }
    }
    return elems;
}

template<typename E>
IndexByVertexId ElementsGroup<E>::getIndexByVertexId() const {
    IndexByVertexId res;
    for (UInt i = 0; i < this->size(); i++) {
        const E* e = this->get(i);
        vector<CoordinateId> ids(e->getNumberOfVertices());
        for (UInt j = 0; j < e->getNumberOfVertices(); j++) {
            ids[j] = e->getVertex(j)->getId();
        }
        res.insert(pair<vector<CoordinateId>,const E*>(ids,e));
    }
    return res;
}


template<typename E>
vector<UInt> ElementsGroup<E>::getElemsWith_(
        const vector<LayerId>& lays) const {

    set<LayerId> layIds(lays.begin(), lays.end());
    vector<UInt> elems;
    elems.reserve(this->size());
    for (UInt i = 0; i < this->size(); i++) {
        if (layIds.count(this->get(i)->getLayerId()) == 1) {
            elems.push_back(i);
        }
    }
    return elems;
}
