#include "gtest/gtest.h"

#include "physicalModel/Predefined.h"
#include "model/Model.h"

using ModelObject = SEMBA::Model::Model;

using namespace SEMBA;
using namespace Model;

TEST(ModelTest, CanCreate) {
	ModelObject model = ModelObject();

	EXPECT_NE(&model, nullptr);

	EXPECT_NE(&model.unstructuredMesh, nullptr);
	EXPECT_TRUE(model.unstructuredMesh.coords().empty());
	EXPECT_TRUE(model.unstructuredMesh.elems().empty());
	EXPECT_TRUE(model.unstructuredMesh.layers().empty());

	EXPECT_TRUE(model.physicalModels.empty());
}

TEST(ModelTest, CanInitializeGrid) {
	ModelObject model = ModelObject();

	CoordR3Group coordinatesGroup = CoordR3Group();
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			Coordinate::Id(),
			Math::CVecR3(0.0, 0.0, 0.0)
		)
	);
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			Coordinate::Id(), 
			Math::CVecR3(1.0, 2.0, 3.0)
		)
	);

	const CoordR3* coordinatesArgumentList[1] = {coordinatesGroup.getId(Coordinate::Id(1))};
	ElemRGroup elementsGroup = ElemRGroup();
	elementsGroup.addAndAssignId(
		std::make_unique<NodR>(
			Element::Id(), 
			coordinatesArgumentList
		)
	);

	Mesh::Unstructured mesh = Mesh::Unstructured(coordinatesGroup, elementsGroup);
	model.unstructuredMesh = mesh;

	EXPECT_FALSE(model.unstructuredMesh.coords().empty());
	EXPECT_EQ(2, model.unstructuredMesh.coords().size());

	EXPECT_EQ(
		Math::CVecR3(1.0, 2.0, 3.0), 
		(model.unstructuredMesh.coords()).getId(Coordinate::Id(2))->pos()
	);

	EXPECT_FALSE(model.unstructuredMesh.elems().empty());
	EXPECT_EQ(1, model.unstructuredMesh.elems().size());

	EXPECT_TRUE(model.unstructuredMesh.layers().empty());
}

TEST(ModelTest, CanInitializePhysicalModels) {
	ModelObject model = ModelObject();

	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.addAndAssignId(
		std::make_unique<PhysicalModel::PEC>(
			PhysicalModel::Id(),
			"Material PEC"
		)
	);

	model.physicalModels = physicalModelsGroup;

	EXPECT_EQ(1, model.physicalModels.size());
	EXPECT_EQ(
		"Material PEC",
		model.physicalModels.getId(PhysicalModel::Id(1))->getName()
	);
}
