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
                    eG.addId(new Tri3(ElemId(0), coords.data(), lay.get()));
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
    res.mesh = new Geometry::Mesh::Geometric(Geometry::Grid3(), cG, eG, lG);

    using namespace PhysicalModel;

    res.physicalModels.add(std::make_unique<PEC>(Id(1), "PEC"));
    auto const* pec = res.physicalModels.getId(Id(1));
    res.mesh->castTo<Geometry::Mesh::Geometric>()->elems().setModel(pec);
    res.sources = new Source::Group<>();
    res.outputRequests = new OutputRequest::Group<>();

    return res;
}

Geometry::Mesh::Unstructured Parser::readAsUnstructuredMesh() const 
{
    Geometry::CoordR3Group cG = readCoordinates(this->filename);
    ElemRGroup eG;
    std::unique_ptr<Layer::Layer> lay;
    LayerGroup lG;
    std::tie(lay, eG) = readLayerAndElements(this->filename, cG);
    if (lay) {
        lG.addAndAssignId(std::move(lay));
    }

    return Mesh::Unstructured(cG, eG, lG);
}

} /* namespace STL */
} /* namespace Parser */
} /* namespace SEMBA */
