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
namespace Util {
namespace Wire {

template<class T>
Group<T>::Group(const Data& smb) {
    init_(smb);
}

template<class T>
Group<T>::~Group() {
    for (std::size_t i = 0; i < wires_.size(); i++) {
        delete wires_[i];
    }
    for (std::map<MatId, PhysicalModel::Wire::Extremes*>::iterator
         it = mats_.begin(); it != mats_.end(); ++it) {
        delete it->second;
    }
}

template<class T>
void Group<T>::printInfo() const {
    std::cout << "--- Group Info ---" << std::endl;
    std::cout << "Wires:" << std::endl;
    for (std::size_t i = 0; i < wires_.size(); i++) {
        wires_[i]->printInfo();
    }
    std::cout << "Mats:" << std::endl;
    for (std::map<MatId, PhysicalModel::Wire::Extremes*>::const_iterator
         it = mats_.begin(); it != mats_.end(); ++it) {
        it->second->printInfo();
    }
}

template<class T>
void Group<T>::init_(const Data& smb) {
    const Graph& graph = constructGraph_(smb);
    fillWiresInfo_(graph, smb);
}

template<class T>
typename Group<T>::Graph Group<T>::constructGraph_(const Data& smb) {

    Geometry::Element::Group<const Geometry::Element::Line<T>> wires;
    const PhysicalModel::Group<>& mats = *smb.physicalModels;
    {
        Geometry::Element::Group<const Geometry::Element::Line<T>> lines;
        if (std::is_floating_point<T>::value) {
            lines = smb.mesh->castTo<Geometry::Mesh::Unstructured>()
                        ->elems().getOf<Geometry::Element::Line<T>>();
        } else if (std::is_integral<T>::value) {
            lines = smb.mesh->castTo<Geometry::Mesh::Structured>()
                        ->elems().getOf<Geometry::Element::Line<T>>();
        }
        PhysicalModel::Group<const PhysicalModel::Wire::Wire>
            pmwires = mats.getOf<PhysicalModel::Wire::Wire>();
        std::vector<MatId> pmwiresIds = pmwires.getIds();
        PhysicalModel::Group<const PhysicalModel::Multiport::Multiport>
            pmmults = mats.getOf<PhysicalModel::Multiport::Multiport>();
        std::vector<MatId> pmmultsIds = pmmults.getIds();
        std::vector<MatId> matIds;
        matIds.reserve(pmwiresIds.size() + pmmultsIds.size());
        matIds.insert(matIds.end(), pmwiresIds.begin(), pmwiresIds.end());
        matIds.insert(matIds.end(), pmmultsIds.begin(), pmmultsIds.end());
        wires = lines.getMatId(matIds);
    }
    Graph graph;
    graph.init(wires);
    const typename Graph::GraphBound* nodePtr;
    for (std::size_t i = 0; i < graph.numBounds(); i++) {
        nodePtr = graph.bound(i);
        if (nodePtr->numBounds() > 2) {
            graph.splitBound(i);
        } else if (nodePtr->numBounds() == 2) {
            Geometry::LayerId layId[2];
            MatId matId[2];
            bool isWireMat[2];
            bool isWireExtrMat[2];
            bool leaving[2];
            for (std::size_t j = 0; j < 2; j++) {
                layId[j] = nodePtr->getBound(j)->elem()->getLayerId();
                matId[j] = nodePtr->getBound(j)->elem()->getMatId();
                isWireMat[j] =
                    mats.getId(matId[j])->is<PhysicalModel::Wire::Wire>();
                isWireExtrMat[j] =
                    mats.getId(matId[j])->is<PhysicalModel::Wire::Extremes>();
                if (nodePtr->elem()->getId() ==
                        nodePtr->getBound(j)->getBound(0)->elem()->getId()) {
                    leaving[j] = true;
                } else {
                    leaving[j] = false;
                }
            }
            bool extreme = leaving[0] && leaving[1];
            if (layId[0] != layId[1]) {
                graph.splitBound(i);
            } else if (isWireExtrMat[0] || isWireExtrMat[1]) {
                if ((!isWireExtrMat[0] || !isWireExtrMat[1]) ||
                    (matId[0] != matId[1]) || (leaving[0] == leaving[1])) {
                    graph.splitBound(i);
                }
            } else if (isWireMat[0] && isWireMat[1]) {
                if ((matId[0] != matId[1]) || (leaving[0] == leaving[1])) {
                    graph.splitBound(i);
                }
            } else if (!isWireMat[0] && !isWireMat[1]) {
                if ((matId[0] != matId[1]) || extreme) {
                    graph.splitBound(i);
                }
            } else if ((!isWireMat[0] && leaving[0]) ||
                       (!isWireMat[1] && leaving[1])) {
                graph.splitBound(i);
            }
        }
    }
    return graph;
}

template<class T>
void Group<T>::fillWiresInfo_(const typename Group<T>::Graph& graph,
                              const Data& smb) {
    std::vector<std::vector<const Geometry::Element::Line<T>*>> wires =
        getLines_(graph);
    const PhysicalModel::Wire::Wire* wireMat;
    const PhysicalModel::Multiport::Multiport *extremeL, *extremeR;
    std::map<const PhysicalModel::Wire::Wire*, std::size_t> numWireMat;
    for (std::size_t i = 0; i < wires.size(); i++) {
        wireMat = NULL;
        extremeL = extremeR = NULL;
        getWireMats_(wireMat, extremeL, extremeR, wires[i], smb);
        if (wireMat == NULL) {
            continue;
        }
        PhysicalModel::Wire::Extremes* wireExtremes;
        if (wireMat->is<PhysicalModel::Wire::Extremes>()) {
            wireExtremes = wireMat->cloneTo<PhysicalModel::Wire::Extremes>();
        } else {
            if (numWireMat.count(wireMat) == 0) {
                numWireMat[wireMat] = 0;
            }
            std::stringstream aux;
            aux << wireMat->getName() << "_" << ++numWireMat[wireMat];
            wireExtremes = new PhysicalModel::Wire::Extremes(aux.str(),
                                                             *wireMat,
                                                             extremeL,
                                                             extremeR);
        }
        wireExtremes->setId(MatId(i + 1));
        mats_[wireExtremes->getId()] = wireExtremes;
        Geometry::Element::Polyline<T>* newWire = newWire_(wires[i],
                                                           wireExtremes);
        if (newWire != NULL) {
            wires_.push_back(newWire);
        }
    }
}

template<class T>
std::vector<std::vector<const Geometry::Element::Line<T>*>>
    Group<T>::getLines_(
        const typename Group<T>::Graph& graph) {
    std::vector<std::vector<const Geometry::Element::Line<T>*>> res;
    std::set<Geometry::ElemId> visLines;
    const typename Graph::GraphElem *prevSegment, *nextSegment;
    Geometry::ElemId prvNodeId;
    for (std::size_t n = 1; n < 3; n++) {
        for (std::size_t i = 0; i < graph.numElems(); i++) {
            if ((graph.elem(i)->getBound(0)->numBounds() == n) &&
                (visLines.count(graph.elem(i)->elem()->getId()) == 0)) {
                std::vector<const Geometry::Element::Line<T>*> wireLines;
                nextSegment = graph.elem(i);
                while (nextSegment != NULL) {
                    if (visLines.count(nextSegment->elem()->getId()) != 0) {
                        break;
                    }
                    wireLines.push_back(nextSegment->elem());
                    visLines.insert(nextSegment->elem()->getId());
                    prevSegment = nextSegment;
                    nextSegment = NULL;
                    for (std::size_t
                         j = 0; j < prevSegment->numNeighbors(); j++) {
                        if (visLines.count(
                                prevSegment->getNeighbor(j)
                                    ->elem()->getId()) == 0) {
                            nextSegment = prevSegment->getNeighbor(j);
                        }
                    }
                    if (nextSegment == NULL) {
                        break;
                    }
                }
                if (!wireLines.empty()) {
                    res.push_back(wireLines);
                }
            }
        }
    }
    return res;
}

template<class T>
void Group<T>::getWireMats_(
        const PhysicalModel::Wire::Wire*& wireMat,
        const PhysicalModel::Multiport::Multiport*& extremeL,
        const PhysicalModel::Multiport::Multiport*& extremeR,
        const std::vector<const Geometry::Element::Line<T>*>& lines,
        const Data& smb) {

    const PhysicalModel::Group<>& mats = *smb.physicalModels;
    if (lines.empty()) {
        return;
    }
    std::vector<MatId> matIds;
    matIds.push_back(lines[0]->getMatId());
    for (std::size_t i = 1; i < lines.size(); i++) {
        if (lines[i]->getMatId() != matIds.back()) {
            matIds.push_back(lines[i]->getMatId());
        }
    }

    wireMat = NULL;
    extremeL = NULL;
    extremeR = NULL;
    for (std::size_t m = 0; m < matIds.size(); m++) {
        if (mats.getId(matIds[m])->is<PhysicalModel::Multiport::Multiport>()) {
            if ((extremeL == NULL) &&
                (wireMat == NULL)) {
                extremeL =
                    mats.getId(matIds[m])->castTo<
                        PhysicalModel::Multiport::Multiport>();
            } else if ((extremeL != NULL) &&
                       (wireMat == NULL)) {
                //Never happens
            } else if ((wireMat != NULL) &&
                       (extremeR == NULL)) {
                extremeR =
                    mats.getId(matIds[m])->castTo<
                        PhysicalModel::Multiport::Multiport>();
            } else if (extremeR != NULL) {
                //Never happens
            }
        } else if (mats.getId(matIds[m])->is<PhysicalModel::Wire::Wire>()) {
            if (wireMat == NULL) {
                wireMat =
                    mats.getId(matIds[m])->castTo<PhysicalModel::Wire::Wire>();
            } else if (wireMat != NULL) {
                //Never happens
            }
        }
    }
    if (wireMat == NULL) {
        std::stringstream aux;
        aux << "ERROR @ Wire::Group: Lines with ids";
        for (std::size_t i = 0; i < lines.size(); i++) {
            if (i == 0) {
                aux << " ";
            } else if (i + 1 == lines.size()) {
                aux << " and ";
            } else {
                aux << ", ";
            }
            aux << lines[i]->getId();
        }
        aux << " specify a incorrect Wire";
        throw std::logic_error(aux.str());
    }
}

template<class T>
Geometry::Element::Polyline<T>*
    Group<T>::newWire_(
        const std::vector<const Geometry::Element::Line<T>*>& lines,
        const PhysicalModel::PhysicalModel* mat) {
    if (lines.empty()) {
        return NULL;
    }
    std::vector<const Geometry::Coordinate::Coordinate<T, 3>*> v;
    std::vector<Geometry::ElemId> wireIds;
    wireIds.reserve(lines.size());
    const Geometry::Coordinate::Coordinate<T, 3>* prev = lines[0]->getV(0);
    if (lines.size() > 1) {
        if ((lines[0]->getV(0)->getId() == lines[1]->getV(0)->getId()) ||
            (lines[0]->getV(0)->getId() == lines[1]->getV(1)->getId())) {
            prev = lines[0]->getV(1);
        }
    }
    v.push_back(prev);
    for (std::size_t i = 0; i < lines.size(); i++) {
        wireIds.push_back(lines[i]->getId());
        if (prev->getId() != lines[i]->getV(0)->getId()) {
            prev = lines[i]->getV(0);
            v.push_back(prev);
        } else if (prev->getId() != lines[i]->getV(1)->getId()) {
            prev = lines[i]->getV(1);
            v.push_back(prev);
        }
    }
    const Geometry::Layer::Layer* lay = lines[0]->getLayer();
    wiresIds_.push_back(wireIds);
    std::vector<bool> wireRev;
    wireRev.resize(lines.size());
    bool rev = false;
    if (lines.size() == 1) {
        wireRev[0] = false;
    } else {
        for (std::size_t i = 0; i < lines.size(); i++) {
            if (dynamic_cast<const PhysicalModel::Wire::Wire*>(
                    lines[i]->getModel()) != NULL) {
                if (i == 0) {
                    std::size_t j = i + 1;
                    if ((lines[i]->getV(0)->getId() ==
                            lines[j]->getV(0)->getId()) ||
                        (lines[i]->getV(0)->getId() ==
                            lines[j]->getV(1)->getId())) {
                        rev = true;
                    } else {
                        rev = false;
                    }
                } else {
                    std::size_t j = i - 1;
                    if ((lines[i]->getV(0)->getId() ==
                            lines[j]->getV(0)->getId()) ||
                        (lines[i]->getV(0)->getId() ==
                            lines[j]->getV(1)->getId())) {
                        rev = false;
                    } else {
                        rev = true;
                    }
                }
                break;
            }
        }
        for (std::size_t i = 0; i < lines.size(); i++) {
            wireRev[i] = false;
            if (i == 0) {
                std::size_t j = i + 1;
                if (rev ==
                    ((lines[i]->getV(1)->getId() ==
                        lines[j]->getV(0)->getId()) ||
                     (lines[i]->getV(1)->getId() ==
                        lines[j]->getV(1)->getId()))) {
                    wireRev[i] = true;
                }
            } else {
                std::size_t j = i - 1;
                if (rev ==
                    ((lines[i]->getV(0)->getId() ==
                        lines[j]->getV(0)->getId()) ||
                     (lines[i]->getV(0)->getId() ==
                        lines[j]->getV(1)->getId()))) {
                    wireRev[i] = true;
                }
            }
        }
    }
    wiresRev_.push_back(wireRev);
    if (rev) {
        std::reverse(v.begin(), v.end());
    }
    return new Geometry::Element::Polyline<T>(
                    Geometry::ElemId(wires_.size() + 1), v, lay, mat);
}

}
}
}
