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

#include "Group.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<typename E>
Group<E>& Group<E>::operator=(SEMBA::Group::Group<E>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Identifiable<E, Id>::operator=(rhs);
    return *this;
}

template<typename E>
Group<E>& Group<E>::operator=(SEMBA::Group::Group<E>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Identifiable<E, Id>::operator=(std::move(rhs));
    return *this;
}

template<typename E>
Group<E>& Group<E>::operator=(const SEMBA::Group::Group<E>& rhs) {
    return operator=(SEMBA::Group::Group<E>(rhs));
}

template<typename E>
bool Group<E>::isLinear() const {
    for (std::size_t i = 0; i < this->size(); i++) {
        if (!this->get(i)->isLineal()) {
            return false;
        }
    }
    return true;
}

template<typename E>
Group<E> Group<E>::getMatId(const MatId matId) {
    std::vector<MatId> aux;
    aux.push_back(matId);
    return getMatId(aux);
}

template<typename E>
Group<E> Group<E>::getMatId(
        const std::vector<MatId>& matIds) {

    return this->get(getElemsWith_(matIds));
}

template<typename E>
Group<const E> Group<E>::getMatId(
        const MatId matId) const {
    std::vector<MatId> aux;
    aux.push_back(matId);
    return getMatId(aux);
}

template<typename E>
Group<const E> Group<E>::getMatId(
        const std::vector<MatId>& matIds) const {

    return this->get(getElemsWith_(matIds));
}

template<typename E>
Group<E> Group<E>::getLayerId(const LayerId layerId) {
    std::vector<LayerId> aux;
    aux.push_back(layerId);
    return getLayerId(aux);
}

template<typename E>
Group<E> Group<E>::getLayerId(
        const std::vector<LayerId>& layIds) {

    return this->get(getElemsWith_(layIds));
}

template<typename E>
Group<const E> Group<E>::getLayerId(
        const LayerId layerId) const {
    std::vector<LayerId> aux;
    aux.push_back(layerId);
    return getLayerId(aux);
}

template<typename E>
Group<const E> Group<E>::getLayerId(
        const std::vector<LayerId>& layIds) const {

    return this->get(getElemsWith_(layIds));
}

template<typename E>
Group<E> Group<E>::getMatLayerId(const MatId   matId,
                                         const LayerId layId) {
    return this->get(getElemsWith_(matId, layId));
}

template<typename E>
Group<const E> Group<E>::getMatLayerId(const MatId   matId,
                                               const LayerId layId) const {
    return this->get(getElemsWith_(matId, layId));
}

template<typename E>
std::vector<Id> Group<E>::getIdsWithoutMaterialId(
        const MatId matId) const {

    std::vector<Id> res;
    res.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if (this->get(i)->getMatId() != matId) {
            res.push_back(this->get(i)->getId());
        }
    }
    return res;
}

template<typename E>
Group<const ElemR> Group<E>::getInsideBound(
        const BoxR3& bound) const {
    Group<const ElemR> res;
    res.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        BoxR3 localBound = this->get(i)->getBound();
        if (localBound <= bound) {
            res.add(this->get(i));
        }
    }
    return res;
}

template<typename E>
std::vector<std::pair<const E*, std::size_t> > Group<E>::getElementsWithVertex(
        const CoordId vertexId) const {

    std::vector<std::pair<const E*, std::size_t> > res;
    Group<ElemR> elemsR = this->template getGroupOf<ElemR>();
    for (std::size_t i = 0; i < elemsR.size(); i++) {
        for (std::size_t j = 0; j < elemsR(i)->numberOfVertices(); j++) {
            if ((elemsR(i)->getVertex(j)->getId() == vertexId) &&
                    elemsR(i)->template is<E>()) {

                std::pair<const E*, std::size_t> aux(
                    elemsR(i)->template castTo<E>(), j);
                res.push_back(aux);
            }
        }
    }
    Group<ElemI> elemsI = this->template getGroupOf<ElemI>();
    for (std::size_t i = 0; i < elemsI.size(); i++) {
        for (std::size_t j = 0; j < elemsI(i)->numberOfVertices(); j++) {
            if ((elemsI(i)->getVertex(j)->getId() == vertexId) &&
                    elemsI(i)->template is<E>()) {

                std::pair<const E*, std::size_t> aux(
                    elemsI(i)->template castTo<E>(), j);
                res.push_back(aux);
            }
        }
    }
    return res;
}

template<typename E>
BoxR3 Group<E>::getBound() const {
    if (this->size() == 0) {
        return BoxR3().setInfinity();
    }
    BoxR3 bound;
    {
        Group<const ElemR> elems = this->template getOf<ElemR>();
        for (std::size_t i = 0; i < elems.size(); i++) {
            bound << elems(i)->getBound();
        }
    }
    {
        Group<const ElemI> elemsI = this->template getOf<ElemI>();
        for (std::size_t i = 0; i < elemsI.size(); i++) {
            BoxI3 boxI = elemsI(i)->getBound();
            Math::CVecI3 min = boxI.getMin();
            Math::CVecI3 max = boxI.getMax();
            bound << BoxR3(Math::CVecR3(min(Math::Constants::x),
                                        min(Math::Constants::y),
                                        min(Math::Constants::z)),
                           Math::CVecR3(max(Math::Constants::x),
                                        max(Math::Constants::y),
                                        max(Math::Constants::z)));
        }
    }
    return bound;
}

template<typename E>
BoxR3 Group<E>::getBound(const std::vector<Face>& border) const {
    if (border.size() == 0) {
        return BoxR3().setInfinity();
    }
    BoxR3 bound;
    for (std::size_t i = 0; i < border.size(); i++) {
        const VolR* vol = border[i].first;
        const std::size_t face = border[i].second;
        bound << vol->getBoundOfFace(face);
    }
    return bound;
}

template<typename E>
const CoordR3* Group<E>::getClosestVertex(const Math::CVecR3 pos) const {
    const CoordR3* res = NULL;
    Math::Real minDist = std::numeric_limits<Math::Real>::infinity();
    Group<const ElemR> elems = this->template getOf<ElemR>();
    for (std::size_t b = 0; b < elems.size(); b++) {
        for (std::size_t i = 0; i < elems(i)->numberOfCoordinates(); i++) {
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
void Group<E>::setModel(const Model* newMat) {
    for (std::size_t i = 0; i < this->size(); i++) {
        this->get(i)->setModel(newMat);
    }
}

template<typename E>
void Group<E>::setLayer(const Layer* newLay) {
    for (std::size_t i = 0; i < this->size(); i++) {
        this->get(i)->setLayer(newLay);
    }
}

template<typename E>
void Group<E>::setModel(const Id id,
                        const Model* newMat) {
    this->getId(id)->setModel(newMat);
}

template<typename E>
void Group<E>::setLayer(const Id id,
                        const Layer* newLay) {
    this->getId(id)->setLayer(newLay);
}

template<typename E>
void Group<E>::removeMatId(const MatId matId) {
    std::vector<MatId> aux;
    aux.push_back(matId);
    removeMatId(aux);
}

template<typename E>
void Group<E>::removeMatId(const std::vector<MatId>& matId) {
    Group<E>::remove(getElemsWith_(matId));
}

template<typename E> template<class T>
void Group<E>::reassignPointers(
        const Coordinate::Group< Coordinate::Coordinate<T,3> >& vNew) {

    for (std::size_t i = 0; i < this->size(); i++) {
        if (this->get(i)->template is< Element<T> >()) {
            Element<T>* elem = this->get(i)->template castTo< Element<T> >();
            for (std::size_t j = 0; j < elem->numberOfCoordinates(); j++) {
                CoordId vId = elem->getV(j)->getId();
                elem->setV(j, vNew.getId(vId));
            }
        }
    }
}

template<typename E>
void Group<E>::reassignPointers(
        const SEMBA::Geometry::Layer::Group<Layer>& lNew) {
    for (std::size_t i = 0; i < this->size(); i++) {
        if (this->get(i)->getLayer() != NULL) {
            this->get(i)->setLayer(lNew.getId(this->get(i)->getLayerId()));
        }
    }
}

template<typename E>
void Group<E>::reassignPointers(
        const SEMBA::Group::Identifiable<Model,MatId>& mNew) {
    for (std::size_t i = 0; i < this->size(); i++) {
        if (this->get(i)->getModel() != NULL) {
            this->get(i)->setModel(mNew.getId(this->get(i)->getModelId()));
        }
    }
}

template<typename E>
std::map<LayerId, std::vector<const E*> >
        Group<E>::separateByLayers() const {
    std::map<LayerId, std::vector<const E*> > res;
    for (std::size_t i = 0; i < this->size(); i++) {
        const LayerId layerId = this->get(i)->getLayerId();
        typename std::map<LayerId, std::vector<const E*> >::iterator it =
            res.find(layerId);
        if (it == res.end()) {
            std::pair<LayerId, std::vector<const E*> > newEntry;
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
void Group<E>::linearize() {
    if (isLinear()) {
        return;
    }

    for(std::size_t i = 0; i < this->size(); i++) {
        if (!this->get(i)->isLineal()) {
            this->set(i, this->get(i)->linearize());
        }
    }
}

template<typename E>
void Group<E>::printInfo() const {
    std::cout << "--- Group info ---" << std::endl;
}

template<typename E>
std::vector<std::size_t> Group<E>::getElemsWith_(
        const std::vector<MatId>& mats) const {

    std::set<MatId> matIds(mats.begin(), mats.end());
    std::vector<std::size_t> elems;
    elems.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if (matIds.count(this->get(i)->getMatId()) == 1) {
            elems.push_back(i);
        }
    }
    return elems;
}

template<typename E>
IndexByVertexId Group<E>::getIndexByVertexId() const {
    IndexByVertexId res;
    for (std::size_t i = 0; i < this->size(); i++) {
        const E* e = this->get(i);
        std::vector<CoordId> ids(e->numberOfVertices());
        for (std::size_t j = 0; j < e->numberOfVertices(); j++) {
            ids[j] = e->getVertex(j)->getId();
        }
        res.insert(std::pair<std::vector<CoordId>,const E*>(ids,e));
    }
    return res;
}


template<typename E>
std::vector<std::size_t> Group<E>::getElemsWith_(
        const std::vector<LayerId>& lays) const {

    std::set<LayerId> layIds(lays.begin(), lays.end());
    std::vector<std::size_t> elems;
    elems.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if (layIds.count(this->get(i)->getLayerId()) == 1) {
            elems.push_back(i);
        }
    }
    return elems;
}

template<typename E>
std::vector<std::size_t> Group<E>::getElemsWith_(
        const MatId& matId, const LayerId& layId) const {

    std::vector<std::size_t> elems;
    elems.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        if ((this->get(i)->getLayerId() == layId) &&
            (this->get(i)->getMatId()   == matId)) {
            elems.push_back(i);
        }
    }
    return elems;
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
