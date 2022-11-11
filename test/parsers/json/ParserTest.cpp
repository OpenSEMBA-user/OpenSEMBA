#include "gtest/gtest.h"

#include "parsers/json/Parser.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Tetrahedron4.h"
#include "geometry/element/Hexahedron8.h"
#include "math/function/Gaussian.h"
#include "outputRequest/FarField.h"
#include "outputRequest/OnPoint.h"
#include "outputRequest/BulkCurrent.h"
#include "physicalModel/wire/Wire.h"
#include "physicalModel/multiport/RLC.h"

using namespace SEMBA;
using namespace SEMBA::Parsers::JSON;
using namespace Geometry::Mesh;

class ParserJSONParserTest : public ::testing::Test {
protected:
    static std::string getFilename(const std::string& name)
    {
        return "testData/" + name + ".gid/" + name + ".dat";
    }
};

TEST_F(ParserJSONParserTest, DMCWF) 
{
    ASSERT_NO_THROW(Parser(getFilename("dmcwf")).read());
}

TEST_F(ParserJSONParserTest, Sphere) 
{
    Data data;
    ASSERT_NO_THROW(data = Parser(getFilename("sphere")).read());

    EXPECT_EQ(Math::CVecI3(20), data.grids.getNumCells());
}

TEST_F(ParserJSONParserTest, RectilinearGrid) 
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended-rectilinear.smb.json");
    auto data{ jsonParser.readExtended() };
    EXPECT_EQ(Math::CVecI3(1,2,3), data.grids.getNumCells());
    EXPECT_EQ(std::vector<Math::Real>({0, 1}), data.grids.getPos(0));
    EXPECT_EQ(std::vector<Math::Real>({0, 1, 2}), data.grids.getPos(1));
    EXPECT_EQ(std::vector<Math::Real>({0, 1, 2, 3}), data.grids.getPos(2));
}

TEST_F(ParserJSONParserTest, SphereExtended)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended.smb.json");

    auto data = jsonParser.readExtended();

    EXPECT_EQ(data.grids.getNumCells(), Math::CVecR3(51, 23, 15));

    auto& sources = data.sources;
    EXPECT_EQ(sources.size(), 1);

    const Source::PlaneWave* source = sources.get()[0]->castTo<Source::PlaneWave>();
    EXPECT_EQ(
        source->getPolarization(), 
        Math::CVecR3(-0.4082482904638631, 0.8164965809277261, -0.4082482904638631)
    );
    EXPECT_EQ(
        source->getDirection(), 
        Math::CVecR3(1.0, 1.0, 1.0)
    );

    Source::Magnitude::Magnitude magnitude = *source->getMagnitude();
    EXPECT_EQ(
        magnitude, 
        Source::Magnitude::Magnitude(
            new Math::Function::Gaussian(
                Math::Function::Gaussian::buildFromMaximumFrequency(
                    1000000000.0,
                    1.0
                )
            )
        )
    );

	auto& analysis = data.analysis;
	EXPECT_NE(NULL, analysis);

	std::ifstream input("testData/sphere.gid/sphere-extended.smb.json");
	json j;
	input >> j;

	EXPECT_EQ(j["analysis"], analysis);

	for (auto& element : j["analysis"].items()) {
		EXPECT_EQ(element.value(), analysis[element.key()]);
	}

    auto& model = data.model;

    // 2 predefined + 6 bounds
    EXPECT_EQ(8, model.physicalModels.size());
    EXPECT_EQ("pec", model.physicalModels.get()[0]->getName());
    EXPECT_EQ("pmc", model.physicalModels.get()[1]->getName());

	const PhysicalModel::Bound::Type boundaryLowerUpperMaterials[6] = {
	    PhysicalModel::Bound::Type::pml,
	    PhysicalModel::Bound::Type::pec,
	    PhysicalModel::Bound::Type::pmc,
		PhysicalModel::Bound::Type::mur1,
		PhysicalModel::Bound::Type::mur2,
		PhysicalModel::Bound::Type::periodic
    };
    
    auto& boundaries = model.physicalModels.getOf<PhysicalModel::Bound>();
    for (auto& bound : boundaryLowerUpperMaterials) {
        EXPECT_TRUE(
            std::find_if(
                boundaries.cbegin(),
                boundaries.cend(),
                [&](const PhysicalModel::Bound* boundI) -> bool {return boundI->getType() == bound; }
            ) != boundaries.cend()
        );
    }

    // 384 coming from grid, 8 from source, 1 from point probe and 8 from far field
    // New elements added as part of Boundaries: 6 faces * 4 points/face
    ASSERT_EQ(384 + 8 + 1 + 8 + 24, model.mesh.coords().size());
    EXPECT_EQ(
        Math::CVecR3(2.33333325, -5.71501865e-16, 1.66666663),
        model.mesh.coords().get()[0]->pos()
    );
    EXPECT_EQ(
        Math::CVecR3(1.28204191, -1.31762123e+01, -1.70370862e-01),
        model.mesh.coords().get()[383]->pos()
    );

    auto& probes = data.outputRequests;

    ASSERT_EQ(2, probes.size());
    EXPECT_EQ(OutputRequest::OutputRequest::Type::electric, probes.get()[0]->getType());
    EXPECT_EQ(OutputRequest::OutputRequest::Type::electricFarField, probes.get()[1]->getType());

    ASSERT_EQ(1, probes.get()[0]->getTarget().size());

    auto recoveredNode = probes.get()[0]->getTarget().at(0)->castTo<Geometry::NodR>();
    EXPECT_EQ(
        Math::CVecR3(-0.844136014, 12.0172290, 13.1547242),
        recoveredNode->getV(0)->pos()
    );
}

TEST_F(ParserJSONParserTest, SphereExtendedWithWrongSubversion)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended-wrong-subversion.smb.json");
    try {
        jsonParser.readExtended();
        FAIL() << "No exception was thrown";
    }
    catch (const std::logic_error& exception) {
        EXPECT_STREQ(exception.what(), "File version 0.15a is not supported.");
    }
}

TEST_F(ParserJSONParserTest, SphereExtendedWithOnePlaneFarField)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended-one-plane-farfield.smb.json");

    auto data = jsonParser.readExtended();

    EXPECT_EQ(data.outputRequests.sizeOf<OutputRequest::FarField>(), 1);
    auto farFieldProbe = data.outputRequests.getOf<OutputRequest::FarField>().front();

    EXPECT_EQ(farFieldProbe->initialPhi, 0.0);
    EXPECT_EQ(farFieldProbe->finalPhi, 0.0);
    EXPECT_EQ(farFieldProbe->stepPhi, 0.1 * 2 * Math::Constants::pi / 360.0);
}

TEST_F(ParserJSONParserTest, SphereExtendedWithBulkCurrentProbe) 
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended-bulk-current-probe.smb.json");

    auto data = jsonParser.readExtended();

    EXPECT_EQ(data.outputRequests.sizeOf<OutputRequest::BulkCurrent>(), 1);
    auto bulkCurrentProbe = data.outputRequests.getOf<OutputRequest::BulkCurrent>().front();

    EXPECT_EQ(bulkCurrentProbe->getName(), "Bulk probe");
    EXPECT_EQ(bulkCurrentProbe->getType(), OutputRequest::OutputRequest::Type::bulkCurrentMagnetic);
    EXPECT_EQ(bulkCurrentProbe->getDir(), Math::Constants::CartesianAxis::y);
    EXPECT_EQ(bulkCurrentProbe->getSkip(), 18);
    EXPECT_EQ(bulkCurrentProbe->getTarget().size(), 1);

    auto bulkTarget = bulkCurrentProbe->getTarget().front()->castTo<Geometry::HexR8>();
    auto bound = bulkTarget->getBound();

    EXPECT_EQ(bound.getMax(), Math::CVecR3(0.006, 0.003, 0.005)); // There is a scale factor as JSON is specified in mm
    EXPECT_EQ(bound.getMin(), Math::CVecR3(0.0, -0.002, 0.0));
}

TEST_F(ParserJSONParserTest, Antennas)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/antennas.gid/problem.smb.json");

    auto data = jsonParser.readExtended();

    EXPECT_EQ(data.outputRequests.sizeOf<OutputRequest::OnPoint>(), 3);
    EXPECT_EQ(data.sources.sizeOf<Source::Generator>(), 1);
    EXPECT_EQ(data.model.mesh.elems().sizeOf<Geometry::NodR>(), 6);

    EXPECT_EQ(data.model.physicalModels.size(), 5); // Cable, 2 connector, 2 bounds (pec and pml)

    auto& materialCableList = data.model.physicalModels.getOf<SEMBA::PhysicalModel::Wire::Wire>();
    EXPECT_EQ(materialCableList.size(), 1);

    auto& materialPortList = data.model.physicalModels.getOf<SEMBA::PhysicalModel::Multiport::RLC>();
    EXPECT_EQ(materialPortList.size(), 1);

    auto& materialCable = materialCableList.front();
    auto& materialPort = materialPortList.front();

    SEMBA::Geometry::ElemView elementsWithCableMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialCable->getId()) {
            elementsWithCableMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithCableMaterial.size(), 2);

    SEMBA::Geometry::ElemView elementsWithPortMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialPort->getId()) {
            elementsWithPortMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithPortMaterial.size(), 2);
}

TEST_F(ParserJSONParserTest, AntennasWithProbesUsingCoordIds)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/antennas.gid/problem-probes-with-coordIds.smb.json");

    auto data = jsonParser.readExtended();

    EXPECT_EQ(data.outputRequests.sizeOf<OutputRequest::OnPoint>(), 3);
    EXPECT_EQ(data.sources.sizeOf<Source::Generator>(), 1);
    EXPECT_EQ(data.model.mesh.elems().sizeOf<Geometry::NodR>(), 9);

    EXPECT_EQ(data.model.physicalModels.size(), 5); // Cable, 2 connector, 2 bounds (pec and pml)

    auto& materialCableList = data.model.physicalModels.getOf<SEMBA::PhysicalModel::Wire::Wire>();
    EXPECT_EQ(materialCableList.size(), 1);

    auto& materialPortList = data.model.physicalModels.getOf<SEMBA::PhysicalModel::Multiport::RLC>();
    EXPECT_EQ(materialPortList.size(), 1);

    auto& materialCable = materialCableList.front();
    auto& materialPort = materialPortList.front();

    SEMBA::Geometry::ElemView elementsWithCableMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialCable->getId()) {
            elementsWithCableMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithCableMaterial.size(), 2);

    SEMBA::Geometry::ElemView elementsWithPortMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialPort->getId()) {
            elementsWithPortMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithPortMaterial.size(), 2);
}

TEST_F(ParserJSONParserTest, Wires) 
{
    ASSERT_NO_THROW(Parser(getFilename("wires")).read());
}

TEST_F(ParserJSONParserTest, Bowtie) 
{
    Data data;
    ASSERT_NO_THROW(data = Parser(getFilename("bowtie")).read());

    ASSERT_TRUE(data.mesh != nullptr);
    Unstructured unstrMesh = dynamic_cast<Unstructured&>(*data.mesh);

    EXPECT_EQ(422, unstrMesh.coords().size());
    EXPECT_EQ(836, unstrMesh.elems().size());
}

TEST_F(ParserJSONParserTest, B2)
{
	Data data;
	ASSERT_NO_THROW(data = Parser(getFilename("b2")).read());

	ASSERT_TRUE(data.mesh != nullptr);
	Unstructured unstrMesh = dynamic_cast<Unstructured&>(*data.mesh);

	EXPECT_EQ(361, unstrMesh.coords().size());
	EXPECT_EQ(652, unstrMesh.elems().sizeOf<Geometry::Tri3>());
}
