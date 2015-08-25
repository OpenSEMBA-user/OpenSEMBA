/*
 * GroupElements.hpp
 *
 *  Created on: Mar 28, 2013
 *      Author: luis
 */

#include "GroupElements.h"

template<typename E>
GroupElements<E>& GroupElements<E>::operator=(VectorPtr<E>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<E, ElementId>::operator=(rhs);
    return *this;
}

template<typename E>
GroupElements<E>& GroupElements<E>::operator=(VectorPtr<E>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<E, ElementId>::operator=(std::move(rhs));
    return *this;
}

template<typename E>
GroupElements<E>& GroupElements<E>::operator=(const VectorPtr<E>& rhs) {
    return operator=(VectorPtr<E>(rhs));
}

template<typename E>
bool GroupElements<E>::isLinear() const {
    return (this->template sizeOf<Triangle6 >() == 0 &&
            this->template sizeOf<Tetrahedron10>() == 0);
}

template<typename E>
GroupElements<E> GroupElements<E>::getMatId(const MatId matId) {
    vector<MatId> aux;
    aux.push_back(matId);
    return getMatId(aux);
}

template<typename E>
GroupElements<E> GroupElements<E>::getMatId(
        const vector<MatId>& matIds) {

    return this->get(getElemsWith_(matIds));
}

template<typename E>
GroupElements<const E> GroupElements<E>::getMatId(
        const MatId matId) const {
    vector<MatId> aux;
    aux.push_back(matId);
    return getMatId(aux);
}

template<typename E>
GroupElements<const E> GroupElements<E>::getMatId(
        const vector<MatId>& matIds) const {

    return this->get(getElemsWith_(matIds));
}

template<typename E>
GroupElements<E> GroupElements<E>::getLayerId(const LayerId layerId) {
    vector<LayerId> aux;
    aux.push_back(layerId);
    return getLayerId(aux);
}

template<typename E>
GroupElements<E> GroupElements<E>::getLayerId(
        const vector<LayerId>& layIds) {

    return this->get(getElemsWith_(layIds));
}

template<typename E>
GroupElements<const E> GroupElements<E>::getLayerId(
        const LayerId layerId) const {
    vector<LayerId> aux;
    aux.push_back(layerId);
    return getLayerId(aux);
}

template<typename E>
GroupElements<const E> GroupElements<E>::getLayerId(
        const vector<LayerId>& layIds) const {

    return this->get(getElemsWith_(layIds));
}

template<typename E>
GroupElements<E> GroupElements<E>::getMatLayerId(const MatId   matId,
                                                 const LayerId layId) {
    return getMatId(matId).getLayerId(layId);
}

template<typename E>
GroupElements<const E> GroupElements<E>::getMatLayerId(
        const MatId   matId,
        const LayerId layId) const {
    return getMatId(matId).getLayerId(layId);
}

template<typename E>
vector<ElementId> GroupElements<E>::getIdsWithoutMaterialId(
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
vector<ElementId> GroupElements<E>::getIdsInsideBound(
        const BoxR3& bound) const {

    GroupElements<ElemR> elems = this->template getGroupOf<ElemR>();
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
vector<pair<const E*, UInt> > GroupElements<E>::getElementsWithVertex(
        const CoordinateId vertexId) const {

    vector<pair<const E*, UInt> > res;
    GroupElements<ElemR> elemsR = this->template getGroupOf<ElemR>();
    for (UInt i = 0; i < elemsR.size(); i++) {
        for (UInt j = 0; j < elemsR(i)->numberOfVertices(); j++) {
            if ((elemsR(i)->getVertex(j)->getId() == vertexId) &&
                    elemsR(i)->template is<E>()) {

                pair<const E*, UInt> aux(elemsR(i)->template castTo<E>(), j);
                res.push_back(aux);
            }
        }
    }
    GroupElements<ElemI> elemsI = this->template getGroupOf<ElemI>();
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
BoxR3 GroupElements<E>::getBound() const {
    if (this->size() == 0) {
        return BoxR3().setInfinity();
    }
    BoxR3 bound;
    {
        GroupElements<const ElemR> elems = this->template getOf<ElemR>();
        for (UInt i = 0; i < elems.size(); i++) {
            bound << elems(i)->getBound();
        }
    }
    {
        GroupElements<const ElemI> elemsI = this->template getOf<ElemI>();
        for (UInt i = 0; i < elemsI.size(); i++) {
            BoxI3 boxI = elemsI(i)->getBound();
            CVecI3 min = boxI.getMin();
            CVecI3 max = boxI.getMax();
            bound << BoxR3(
                    CVecR3(min(x),min(y),min(z)),
                    CVecR3(max(x),max(y),max(z)));
        }
    }
    return bound;
}

template<typename E>
BoxR3 GroupElements<E>::getBound(const vector<Face>& border) const {
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
const CoordR3* GroupElements<E>::getClosestVertex(const CVecR3 pos) const {
    const CoordR3* res = NULL;
    Real minDist = numeric_limits<Real>::infinity();
    GroupElements<const ElemR> elems = this->template getOf<ElemR>();
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
void GroupElements<E>::setMatId(const MatId newId) {
    for (UInt i = 0; i < this->size(); i++) {
        this->get(i)->setMatId(newId);
    }
}

template<typename E>
void GroupElements<E>::setLayerId(const LayerId newId) {
    for (UInt i = 0; i < this->size(); i++) {
        this->get(i)->setLayerId(newId);
    }
}

template<typename E>
void GroupElements<E>::setMatId(const ElementId id,
                                const MatId newMatId) {
    this->getId(id)->setMatId(newMatId);
}

template<typename E>
void GroupElements<E>::setLayerId(const ElementId id,
                                  const LayerId newId) {
    this->getId(id)->setLayerId(newId);
}

template<typename E>
void GroupElements<E>::removeMatId(const MatId matId) {
    vector<MatId> aux;
    aux.push_back(matId);
    removeMatId(aux);
}

template<typename E>
void GroupElements<E>::removeMatId(const vector<MatId>& matId) {
    Group<E>::remove(getElemsWith_(matId));
}

template<typename E> template<class T>
void GroupElements<E>::reassignPointers(
        const GroupCoordinates< Coordinate<T,3> >& vNew) {

    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->template is< Element<T> >()) {
            Element<T>* elem = this->get(i)->template castTo< Element<T> >();
            for (UInt j = 0; j < elem->numberOfCoordinates(); j++) {
                CoordinateId vId = elem->getV(j)->getId();
                elem->setV(j, vNew.getId(vId));
            }
        }
    }
}

template<typename E>
map<LayerId, vector<const E*> >
GroupElements<E>::separateByLayers() const {
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
void GroupElements<E>::linearize() {
    if (isLinear()) {
        return;
    }

    vector<UInt> pos;
    vector<Triangle3*> newTri;
    vector<Tetrahedron4*> newTet;
    for(UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->template is<Triangle6>()) {
            pos.push_back(i);
            newTri.push_back(this->get(i)->linearize());
        }
        if (this->element_[i]->template is<Tetrahedron10>()) {
            pos.push_back(i);
            newTet.push_back(this->get(i)->linearize());
        }
    }
    this->remove(pos);
    this->add(newTri);
    this->add(newTet);
}

template<typename E>
void GroupElements<E>::printInfo() const {
    cout << "--- GroupElements info ---" << endl;
    Group<E>::printInfo();
}

template<typename E>
vector<UInt> GroupElements<E>::getElemsWith_(
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
IndexByVertexId GroupElements<E>::getIndexByVertexId() const {
    IndexByVertexId res;
    for (UInt i = 0; i < this->size(); i++) {
        const E* e = this->get(i);
        vector<CoordinateId> ids(e->numberOfVertices());
        for (UInt j = 0; j < e->numberOfVertices(); j++) {
            ids[j] = e->getVertex(j)->getId();
        }
        res.insert(pair<vector<CoordinateId>,const E*>(ids,e));
    }
    return res;
}


template<typename E>
vector<UInt> GroupElements<E>::getElemsWith_(
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
