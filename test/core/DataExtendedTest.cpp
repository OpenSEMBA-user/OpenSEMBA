#include "gtest/gtest.h"

#include "geometry/Grid.h"
#include "source/PlaneWave.h"
#include "math/function/Gaussian.h"
#include "physicalModel/Predefined.h"

#include "DataExtended.h"

using namespace SEMBA;

TEST(DataExtendedTest, CanCreate) {
	DataExtended dataExtended = DataExtended();

	EXPECT_NE(&dataExtended, nullptr);
	EXPECT_TRUE(dataExtended.boundary.empty());
}

TEST(DataExtendedTest, CanInitializeBoundary) {
	DataExtended dataExtended = DataExtended();

	auto pair = std::make_pair<PhysicalModel::Bound, PhysicalModel::Bound>(
		PhysicalModel::Bound(PhysicalModel::Id(), PhysicalModel::Bound::Type::pec),
		PhysicalModel::Bound(PhysicalModel::Id(), PhysicalModel::Bound::Type::pml)
		);

	dataExtended.boundary.push_back(pair);

	EXPECT_EQ(dataExtended.boundary.at(0), pair);
	auto firstElement = dataExtended.boundary.at(0).first;
	EXPECT_EQ(firstElement.getType(), PhysicalModel::Bound::Type::pec);
	EXPECT_EQ(firstElement.getName(), "PEC_Bound");

	auto secondElement = dataExtended.boundary.at(0).second;
	EXPECT_EQ(secondElement.getType(), PhysicalModel::Bound::Type::pml);
	EXPECT_EQ(secondElement.getName(), "PML_Bound");
	EXPECT_EQ(dataExtended.grids, Geometry::Grid3());
	EXPECT_TRUE(dataExtended.sources.empty());
}

TEST(DataExtendedTest, CanInitializeGrids) {
	DataExtended dataExtended = DataExtended();

	auto grid3 = Geometry::Grid3(
		Geometry::BoxR3(
			Math::CVecR3(0.0, 0.0, 0.0),
			Math::CVecR3(1, 1, 1)
		),
		Math::CVecR3(0.5, 0.5, 0.5)
	);

	dataExtended.grids = grid3;

	EXPECT_TRUE(dataExtended.boundary.empty());
	EXPECT_EQ(dataExtended.grids, grid3);
	EXPECT_TRUE(dataExtended.sources.empty());
}

TEST(DataExtendedTest, CanInitializeSources) {
	DataExtended dataExtended = DataExtended();

	Source::Group<> sources = Source::Group<>();

	Math::CVecR3 dir(1.0, 0.0, 0.0);
	Math::CVecR3 pol(0.0, 0.0, 1.0);

	Source::PlaneWave planewave = Source::PlaneWave(
		std::make_unique<Source::Magnitude::Magnitude>(
			new SEMBA::Math::Function::Gaussian(0.5, 0.0, 1.0)
		),
		Geometry::ElemView(),
		dir,
		pol
	);

	sources.addAndAssignId(std::make_unique<Source::PlaneWave>(planewave));

	dataExtended.sources = sources;

	EXPECT_TRUE(dataExtended.boundary.empty());
	EXPECT_EQ(dataExtended.grids, Geometry::Grid3());

	EXPECT_EQ(dataExtended.sources.size(), 1);

	auto sourceInGroup = dataExtended.sources.getId(Source::Id(1))->castTo<Source::PlaneWave>();
	EXPECT_EQ(sourceInGroup->getDirection(), planewave.getDirection());
	EXPECT_EQ(sourceInGroup->getPolarization(), planewave.getPolarization());
	
	EXPECT_EQ(*sourceInGroup->getMagnitude(), *planewave.getMagnitude());
}

TEST(DataExtendedTest, CanInitializeAnalysis) {
	DataExtended dataExtended = DataExtended();

	nlohmann::json analysis = R"({"solver": "ugrfdtd", "someOtherOption": true})"_json;

	dataExtended.analysis = analysis;

	EXPECT_EQ(analysis["solver"], dataExtended.analysis["solver"]);
	EXPECT_EQ(std::string("ugrfdtd"), dataExtended.analysis["solver"].get<std::string>());
	EXPECT_TRUE(dataExtended.analysis["someOtherOption"].get<bool>());
}

TEST(DataExtendedTest, CanInitializeModel) {
	DataExtended dataExtended = DataExtended();

	Geometry::CoordR3Group coordinatesGroup = Geometry::CoordR3Group();
	coordinatesGroup.addAndAssignId(
		std::make_unique<Geometry::CoordR3>(
			Geometry::Coordinate::Id(),
			Math::CVecR3(0.0, 0.0, 0.0)
		)
	);
	coordinatesGroup.addAndAssignId(
		std::make_unique<Geometry::CoordR3>(
			Geometry::Coordinate::Id(),
			Math::CVecR3(1.0, 2.0, 3.0)
		)
	);

	const Geometry::CoordR3* coordinatesArgumentList[1] = { coordinatesGroup.getId(Geometry::Coordinate::Id(1)) };
	Geometry::ElemRGroup elementsGroup = Geometry::ElemRGroup();
	elementsGroup.addAndAssignId(
		std::make_unique<Geometry::NodR>(
			Geometry::Element::Id(),
			coordinatesArgumentList
		)
	);

	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.addAndAssignId(
		std::make_unique<PhysicalModel::PEC>(
			PhysicalModel::Id(),
			"Material PEC"
		)
	);

	const Model::Model model = Model::Model(
		Geometry::Mesh::Unstructured(coordinatesGroup, elementsGroup),
		physicalModelsGroup
	);

	dataExtended.model = model;

	EXPECT_FALSE(dataExtended.model.physicalModels.empty());
	EXPECT_EQ("Material PEC", dataExtended.model.physicalModels.get()[0]->getName());

	EXPECT_FALSE(dataExtended.model.unstructuredMesh.coords().empty());
	EXPECT_EQ(
		Math::CVecR3(1.0, 2.0, 3.0),
		(dataExtended.model.unstructuredMesh.coords().get()[1])->pos()
	);
}
