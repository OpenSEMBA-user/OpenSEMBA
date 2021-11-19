#include "gtest/gtest.h"
#include "geometry/layer/Group.h"

class IdentifiableTest : public ::testing::Test {
public:
    IdentifiableTest() {};
    virtual ~IdentifiableTest() {};

protected:
    std::vector<SEMBA::Geometry::Layer::Layer*> newLayersVector() const;

    void areEqual(
        const std::vector<SEMBA::Geometry::Layer::Layer*>& vec,
        const SEMBA::Geometry::Layer::Group<>& layers);
};

using namespace std;

using namespace SEMBA;
using namespace Class;
using namespace Geometry;

vector<Layer::Layer*> IdentifiableTest::newLayersVector() const {
    std::vector<Layer::Layer*> res;
    res.push_back(new Layer::Layer(LayerId(1), "Patata"));
    res.push_back(new Layer::Layer(LayerId(6), "Cebolla"));
    res.push_back(new Layer::Layer(LayerId(5), "Huevos"));
    return res;
}

void IdentifiableTest::areEqual(
        const std::vector<Layer::Layer*>& vec,
        const Layer::Group<>& layers) {
    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_EQ(*vec[i], *layers.getId(vec[i]->getId()));
    }
}

TEST_F(IdentifiableTest, ctor) {
    vector<Layer::Layer*> vecLayers = IdentifiableTest::newLayersVector();
    Layer::Group<> layers(vecLayers);
    areEqual(vecLayers, layers);
}
