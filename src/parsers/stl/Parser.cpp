#include "Parser.h"

namespace SEMBA {
namespace Parsers {
namespace STL {

using namespace Geometry;
using namespace Math::Constants;

Parser::Parser(const std::string& fn) : 
    SEMBA::Parsers::Parser(fn) 
{

}

CoordR3Group readCoordinates(const std::string& fn)
{
    CoordR3Group cG;

    std::ifstream stl(fn);
    std::string label;
    while (stl.peek() != EOF) {
        stl >> label;
        if (label == "vertex") {
            Math::CVecR3 pos;
            stl >> pos(x) >> pos(y) >> pos(z);
            if (cG.getPos(pos) == nullptr) {
                cG.addPos(pos);
            }
        }
    }  
    
    return cG;
}

std::pair<std::unique_ptr<Layer::Layer>, ElemRGroup> readLayerAndElements(
    const std::string& fn,
    const CoordR3Group& cG)
{
    std::unique_ptr<Layer::Layer> lay;
    ElemRGroup eG;
    
    std::ifstream stl(fn);
    stl.clear();
    while (stl.peek() != EOF) {
        std::string label;
        stl >> label;
        if (label == "solid") {
            std::string layerName;
            stl >> layerName;
            lay = std::make_unique<Layer::Layer>(layerName);
            std::string line;
            while (stl.peek() != EOF) {
                std::getline(stl, line);
                if (line.find("outer loop") != std::string::npos) {
                    std::vector<const CoordR3*> coords;
                    coords.reserve(3);
                    while (stl.peek() != EOF && label != "endloop") {
                        stl >> label;
                        if (label == "vertex") {
                            Math::CVecR3 pos;
                            stl >> pos(x) >> pos(y) >> pos(z);
                            const CoordR3* coord = cG.getPos(pos);
                            if (coord == nullptr) {
                                throw std::runtime_error("Unable to find coordinate during STL reading.");
                            }
                            coords.push_back(coord);
                        }
                    }
                    label.clear();
                    eG.addAndAssignId(
                        std::make_unique<Geometry::Tri3>(
                            Geometry::Tri3(Geometry::ElemId(0), coords.data(), lay.get())
                        )
                    );
                }
            }
        }
    }

    return std::make_pair(std::move(lay), eG);
}

Data Parser::read() const 
{
    Geometry::CoordR3Group cG = readCoordinates(this->filename);
    ElemRGroup eG;
    std::unique_ptr<Layer::Layer> lay;
    std::tie(lay, eG) = readLayerAndElements(this->filename, cG);
    LayerGroup lG;
    lG.addAndAssignId(std::move(lay));
    
    // Stores results and returns.
    Data res;
    res.mesh = std::make_unique<Geometry::Mesh::Geometric>(
        Geometry::Mesh::Geometric(Geometry::Grid3(), cG, eG, lG)
    );


    res.physicalModels.add(std::make_unique<PhysicalModel::PEC>(PhysicalModel::Id(1), "PEC"));
    auto const* pec = res.physicalModels.getId(PhysicalModel::Id(1));

    for (auto& elem : res.mesh->castTo<Geometry::Mesh::Geometric>()->elems()) {
        elem->setModel(pec);
    }

    res.sources = SourceGroup();
    res.outputRequests = OutputRequestGroup();

    return res;
}

Geometry::Mesh::Unstructured Parser::readAsUnstructuredMesh() const 
{
    Geometry::CoordR3Group cG = readCoordinates(this->filename);
    ElemRGroup eG;
    std::unique_ptr<Layer::Layer> lay;
    std::tie(lay, eG) = readLayerAndElements(this->filename, cG);
    LayerGroup lG;
    lG.addAndAssignId(std::move(lay));

    return Mesh::Unstructured(cG, eG, lG);
}

} /* namespace STL */
} /* namespace Parser */
} /* namespace SEMBA */
