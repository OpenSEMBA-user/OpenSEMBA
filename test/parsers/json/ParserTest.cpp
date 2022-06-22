
#include "gtest/gtest.h"

#include "parsers/json/Parser.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Tetrahedron4.h"
#include "math/function/Gaussian.h"
#include "ProblemDescription.h"

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

TEST_F(ParserJSONParserTest, SphereExtended)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended.dat");

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

	std::ifstream input("testData/sphere.gid/sphere-extended.dat");
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
    ASSERT_EQ(384 + 8 + 1 + 8 + 24, model.unstructuredMesh.coords().size());
    EXPECT_EQ(
        Math::CVecR3(2.33333325, -5.71501865e-16, 1.66666663),
        model.unstructuredMesh.coords().get()[0]->pos()
    );
    EXPECT_EQ(
        Math::CVecR3(1.28204191, -1.31762123e+01, -1.70370862e-01),
        model.unstructuredMesh.coords().get()[383]->pos()
    );

    auto& probes = data.outputRequests;

    ASSERT_EQ(2, probes.size());
    EXPECT_EQ(OutputRequest::OutputRequest::Type::electric, probes.get()[0]->getType());
    EXPECT_EQ(OutputRequest::OutputRequest::Type::electricFarField, probes.get()[1]->getType());

    ASSERT_EQ(1, probes.get()[0]->getTarget().size());

    auto recoveredNode = probes.get()[0]->getTarget().at(0)->castTo<Geometry::NodR>();
    EXPECT_EQ(
        Math::CVecR3(-0.8441360141053171, 12.017228978451016, 13.154724231963254),
        recoveredNode->getV(0)->pos()
    );
}

TEST_F(ParserJSONParserTest, SphereExtendedWithWrongSubversion)
{
    SEMBA::Parsers::JSON::Parser jsonParser("testData/sphere.gid/sphere-extended-wrong-subversion.dat");
    try {
        jsonParser.read();
        FAIL() << "No exception was thrown";
    }
    catch (const std::logic_error& exception) {
        EXPECT_STREQ(exception.what(), "File version 0.15a is not supported.");
    }
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
