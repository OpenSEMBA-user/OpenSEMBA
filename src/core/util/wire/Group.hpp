#include "Group.h"

namespace SEMBA {
namespace Util {
namespace Wire {

template<class T>
Group<T>::Group(const Model::UnstructuredModel& model) {
    fillWiresInfo_(
        constructGraph_(
            model.mesh.elems().getOf<Geometry::Element::Line<Math::Real>>(),
            model.physicalModels
        ),
        model.physicalModels
    );
}

template<class T>
Group<T>::Group(const Model::StructuredModel& model) {
    fillWiresInfo_(
        constructGraph_(
            model.mesh.elems().getOf<Geometry::Element::Line<Math::Int>>(),
            model.physicalModels
        ),
        model.physicalModels
    );
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
typename Group<T>::Graph Group<T>::constructGraph_(
    std::vector<const Geometry::Element::Line<T>*>& lines,
    const PMGroup& mats
) {
    // We get the lines with wire or union materials
    std::vector<const Geometry::Element::Line<T>*> wires;

    {        
        std::vector<MatId> matIds;
        for (auto const& mat : mats.getOf<PhysicalModel::Wire::Wire>()) {
            matIds.push_back(mat->getId());
        }
        for (auto const& mat : mats.getOf<PhysicalModel::Multiport::Multiport>()) {
            matIds.push_back(mat->getId());
        }
        
        for (const auto& line : lines) {
            for (const auto& matId : matIds) {
                if (line->getMatId() == matId) {
                    wires.push_back(line);
                    break;
                }
            }
        }
    }

    // The graph of these lines is created
    Graph graph(wires);
    const typename Graph::GraphBound* nodePtr;
    // The previous graph is postprecessed to split the lines of different
    // threads.
    for (std::size_t i = 0; i < graph.numBounds(); i++) {
        nodePtr = graph.bound(i);
        if (nodePtr->numBounds() > 2) {
            // If 3 or more lines are incident to a coordinate then all are
            // distinct wires
            graph.splitBound(i);
        } else if (nodePtr->numBounds() == 2) {
            // In the case of 2 lines incident to a coordinate, they can belong
            // to the same or different wires.
            // The following code checks if they are different, and if so,
            // split them.
            // TODO: Remove plain array
            Geometry::LayerId layId[2];
            MatId matId[2];
            bool isWireMat[2] = { false, false };
            bool isWireExtrMat[2] = { false, false };
			bool leaving[2] = { false, false };
			bool neighEnd[2] = { false, false };
            for (std::size_t j = 0; j < 2; j++) {
                // For each line we obtain:
                // - layId[j]: the LayerId of the line
                // - matId[j]: the MatId
                // - isWireMat[j]: if it has a wire material
                // - isWireExtrMat[j]: if it has a wire material with extremes
                // - leaving[j]: if this line is "going out" of the coordinate
                layId[j] = nodePtr->getBound(j)->elem()->getLayerId();
                matId[j] = nodePtr->getBound(j)->elem()->getMatId();
				neighEnd[j] = (nodePtr->getBound(j)->getBound(0)->numBounds() == 1) ||
					          (nodePtr->getBound(j)->getBound(1)->numBounds() == 1);
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
			bool extreme = (leaving[0] && leaving[1]);

			if (layId[0] != layId[1]) {
				// If the lines have different LayerId, then they are different
				// wires
				graph.splitBound(i);
				continue;
			}

			if (!isWireMat[1] && neighEnd[1] && !leaving[0] &&  leaving[1]) {
				continue;
			}
			if (!isWireMat[1] && neighEnd[1] &&  leaving[0] && leaving[1]) {
				continue;
			}

            if (isWireExtrMat[0] || isWireExtrMat[1]) {
                if ((!isWireExtrMat[0] || !isWireExtrMat[1]) ||
                    (matId[0] != matId[1]) || (leaving[0] == leaving[1])) {
                    // If only a line has a wire material with extremes, or if
                    // both have wire material with extremes and are either
                    // different materials or the direction of the lines does
                    // not match then they are different wires.
                    graph.splitBound(i);
                }
            } else if (isWireMat[0] && isWireMat[1]) {
                if ((matId[0] != matId[1]) || (leaving[0] == leaving[1])) {
                    // If both lines have wire materials and are either
                    // different or the the direction of the lines does not
                    // match then they are different wires.
                    graph.splitBound(i);
                }
            } else if (!isWireMat[0] && !isWireMat[1]) {
                if ((matId[0] != matId[1]) || extreme) {
                    // If both have union materials and are either different
                    // or the direction do not match then they are different
                    // wires.
                    graph.splitBound(i);
                }
            } else if ((!isWireMat[0] && leaving[0]) ||
                       (!isWireMat[1] && leaving[1])) {
					// If any of them are "coming out" and have union material 
					// then they are different wires.
					graph.splitBound(i);
            }
        }
    }
    return graph;
}

template<class T>
void Group<T>::fillWiresInfo_(const typename Group<T>::Graph& graph,
                              const SEMBA::PMGroup& mats) { 
    // First we get the different wires and the lines that make up each one.
    std::vector<std::vector<const Geometry::Element::Line<T>*>> wires =
        getLines_(graph);
    const PhysicalModel::Wire::Wire* wireMat;
    const PhysicalModel::Multiport::Multiport *extremeL, *extremeR;
    // Mapping that for each wire material with extremes that is created is
    // told the number of materials with the same name
    std::map<const PhysicalModel::Wire::Wire*, std::size_t> numWireMat;
    for (std::size_t i = 0; i < wires.size(); i++) {
        wireMat = nullptr;
        extremeL = extremeR = nullptr;
        // We get the wire and union materials that make up the wire.
        getWireMats_(wireMat, extremeL, extremeR, wires[i], mats, graph);
        if (wireMat == nullptr) {
            continue;
        }
        // We get the wire material with extremes of the wire, creating it if
        // it does not exist.
        PhysicalModel::Wire::Extremes* wireExtremes;
        if (wireMat->is<PhysicalModel::Wire::Extremes>()) {
            wireExtremes = new PhysicalModel::Wire::Extremes(
                *wireMat->castTo<PhysicalModel::Wire::Extremes>());
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
        // The wire is created and stored
        Geometry::Element::Polyline<T>* newWire = newWire_(wires[i],
                                                           wireExtremes);
        if (newWire != nullptr) {
            wires_.push_back(newWire);
        }
    }
}

template<class T>
std::vector<std::vector<const Geometry::Element::Line<T>*>>
    Group<T>::getLines_(
        const typename Group<T>::Graph& graph) {
    // This function splits the lines of the graph in wires with the lines that
    // make up each one.
    std::vector<std::vector<const Geometry::Element::Line<T>*>> res;
    std::set<Geometry::ElemId> visLines;
    const typename Graph::GraphElem *prevSegment, *nextSegment;
    Geometry::ElemId prvNodeId;
    for (std::size_t n = 1; n < 3; n++) {
        // First we look for coordinates with only one incident line (n == 1)
        // that has not been visited. After (n == 2) we look for the circular
        // wires.
        for (std::size_t i = 0; i < graph.numElems(); i++) {
            if ((graph.elem(i)->getBound(0)->numBounds() == n) &&
                (visLines.count(graph.elem(i)->elem()->getId()) == 0)) {
                // We enter here when we find an unvisited line whose first
                // coordinate has a number n of incident lines
                std::vector<const Geometry::Element::Line<T>*> wireLines;
                nextSegment = graph.elem(i);
                // We search every line that make up the wire from the line
                // obtained (wireLines)
                while (nextSegment != nullptr) {
                    if (visLines.count(nextSegment->elem()->getId()) != 0) {
                        break;
                    }
                    wireLines.push_back(nextSegment->elem());
                    visLines.insert(nextSegment->elem()->getId());
                    prevSegment = nextSegment;
                    nextSegment = nullptr;
                    for (std::size_t
                         j = 0; j < prevSegment->numNeighbors(); j++) {
                        if (visLines.count(
                                prevSegment->getNeighbor(j)
                                    ->elem()->getId()) == 0) {
                            nextSegment = prevSegment->getNeighbor(j);
                        }
                    }
                    if (nextSegment == nullptr) {
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
        const SEMBA::PMGroup& mats,
        const Graph& graph) {

    if (lines.empty()) {
        return;
    }
    std::vector<MatId> matIds;
    matIds.push_back(lines[0]->getMatId());
    std::set<Geometry::ElemId> linesIds;
    linesIds.insert(lines[0]->getId());
    // We get the different materials in a sorted way that make up the wire
    // and the ElementId of the elements.
    for (std::size_t i = 1; i < lines.size(); i++) {
        linesIds.insert(lines[i]->getId());
        if (lines[i]->getMatId() != matIds.back()) {
            matIds.push_back(lines[i]->getMatId());
        }
    }

    wireMat = nullptr;
    extremeL = nullptr;
    extremeR = nullptr;
    // Algorithm that fills extremeL, wireMat and extremeR.
    // - extremeL: first union material (before wire material)
    // - wireMat: wire material
    // - extremeR: second union material (after the wire material)
    for (std::size_t m = 0; m < matIds.size(); m++) {
        if (mats.getId(matIds[m])->is<PhysicalModel::Multiport::Multiport>()) {
            if ((extremeL == nullptr) &&
                (wireMat == nullptr)) {
                extremeL =
                    mats.getId(matIds[m])->castTo<
                        PhysicalModel::Multiport::Multiport>();
            } else if ((extremeL != nullptr) &&
                       (wireMat == nullptr)) {
                // Never happens
            } else if ((wireMat != nullptr) &&
                       (extremeR == nullptr)) {
                extremeR =
                    mats.getId(matIds[m])->castTo<
                        PhysicalModel::Multiport::Multiport>();
            } else if (extremeR != nullptr) {
                // Never happens
            }
        } else if (mats.getId(matIds[m])->is<PhysicalModel::Wire::Wire>()) {
            if (wireMat == nullptr) {
                wireMat =
                    mats.getId(matIds[m])->castTo<PhysicalModel::Wire::Wire>();
            } else if (wireMat != nullptr) {
                // Never happens
            }
        }
    }
    if (wireMat == nullptr) {
        // If a wire material has not been found then there is an error.
        std::stringstream aux;
        aux << "ERROR @ Wire::Group: Lines with ids";
        // The ElementId of the elements that make up the thread are obtained.
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
        aux << " specify a incorrect Connector. ";
        // The error is because there is only union material. It looks for if
        // there is any line adjacent to the wire lines. If so, the problem is
        // an incorrect normal, otherwise there is no thread material.
        for (std::size_t i = 0; i < graph.numElems(); i++) {
            Geometry::ElemId elemId = graph.elem(i)->elem()->getId();
            const typename Graph::GraphElem* elem = graph.elem(i);
            if ((linesIds.count(elemId) == 0) ||
                (elem->numNeighbors() != 1)) {
                continue;
            }
            for (std::size_t k = 0; k < 2; k++) {
                Geometry::CoordId vId = elem->getBound(k)->elem()->getId();
                for (std::size_t j = 0; j < graph.numBounds(); j++) {
                    const typename Graph::GraphBound* v = graph.bound(j);
                    if ((v->elem()->getId() != vId) ||
                        (v->numBounds() != 1)) {
                        continue;
                    }
                    if (v->getBound(0)->elem()->getId() == elemId) {
                        continue;
                    }
                    const Geometry::Element::Model* model =
                        v->getBound(0)->elem()->getModel();
                    if (model->is<PhysicalModel::Wire::Wire>()) {
                        aux << "Incorrect Normal.";
                        throw std::logic_error(aux.str());
                    }
                }
            }
        }
        aux << "Not attached to a Wire Material.";
        throw std::logic_error(aux.str());
    }
}

template<class T>
Geometry::Element::Polyline<T>*
    Group<T>::newWire_(
        const std::vector<const Geometry::Element::Line<T>*>& lines,
        PhysicalModel::Wire::Extremes* mat) {
    // This function creates the polyline of the wire and also fills the
    // information of the ElementIds that make it up and if the segments are
    // inverted in the original mesh
    if (lines.empty()) {
        return nullptr;
    }
    // We get the coordinates that make up the polyline (v)
    std::vector<const Geometry::Coordinate::Coordinate<T, 3>*> v;
    std::vector<Geometry::ElemId> wireIds;
    wireIds.reserve(lines.size());
    // We see which is the first end
    const Geometry::Coordinate::Coordinate<T, 3>* prev = lines[0]->getV(0);
    if (lines.size() > 1) {
        if ((lines[0]->getV(0)->getId() == lines[1]->getV(0)->getId()) ||
            (lines[0]->getV(0)->getId() == lines[1]->getV(1)->getId())) {
            prev = lines[0]->getV(1);
        }
    }
    v.push_back(prev);
    // From the first end we obtain the rest of coordinates in an sorted way
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
        // We check if the segments with wire material are inverted (rev) in
        // which case we will reverse all the coordinates.
        for (std::size_t i = 0; i < lines.size(); i++) {
            if (dynamic_cast<const PhysicalModel::Wire::Wire*>(
                    lines[i]->getModel()) != nullptr) {
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
        // We check if the segments are inverted with respect to the directions
        // of the segments with wire material
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
        mat->swapExtremes();
    }
    return new Geometry::Element::Polyline<T>(
                    Geometry::ElemId(wires_.size() + 1), v, lay, mat);
}

}
}
}
