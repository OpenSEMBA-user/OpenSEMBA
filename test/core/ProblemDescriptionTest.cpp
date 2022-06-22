#include "gtest/gtest.h"

#include "geometry/Grid.h"
#include "source/PlaneWave.h"
#include "math/function/Gaussian.h"
#include "physicalModel/Predefined.h"
#include "outputRequest/OnPoint.h"

#include "ProblemDescription.h"

using namespace SEMBA;

TEST(ProblemDescriptionTest, CanCreate) {
	ProblemDescription problemDescription = ProblemDescription();

	EXPECT_NE(&problemDescription, nullptr);
}

TEST(ProblemDescriptionTest, CanInitializeGrids) {
	ProblemDescription problemDescription = ProblemDescription();

	auto grid3 = Geometry::Grid3(
		Geometry::BoxR3(
			Math::CVecR3(0.0, 0.0, 0.0),
			Math::CVecR3(1, 1, 1)
		),
		Math::CVecR3(0.5, 0.5, 0.5)
	);

	problemDescription.grids = grid3;

	EXPECT_EQ(problemDescription.grids, grid3);
	EXPECT_TRUE(problemDescription.sources.empty());
}

TEST(ProblemDescriptionTest, CanInitializeSources) {
	ProblemDescription problemDescription = ProblemDescription();

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

	problemDescription.sources = sources;

	EXPECT_EQ(problemDescription.grids, Geometry::Grid3());

	EXPECT_EQ(problemDescription.sources.size(), 1);

	auto sourceInGroup = problemDescription.sources.getId(Source::Id(1))->castTo<Source::PlaneWave>();
	EXPECT_EQ(sourceInGroup->getDirection(), planewave.getDirection());
	EXPECT_EQ(sourceInGroup->getPolarization(), planewave.getPolarization());
	
	EXPECT_EQ(*sourceInGroup->getMagnitude(), *planewave.getMagnitude());
}

TEST(ProblemDescriptionTest, CanInitializeAnalysis) {
	ProblemDescription problemDescription = ProblemDescription();

	nlohmann::json analysis = R"({"solver": "ugrfdtd", "someOtherOption": true})"_json;

	problemDescription.analysis = analysis;

	EXPECT_EQ(analysis["solver"], problemDescription.analysis["solver"]);
	EXPECT_EQ(std::string("ugrfdtd"), problemDescription.analysis["solver"].get<std::string>());
	EXPECT_TRUE(problemDescription.analysis["someOtherOption"].get<bool>());
}

TEST(ProblemDescriptionTest, CanInitializeModel) {
	ProblemDescription problemDescription = ProblemDescription();

	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.addAndAssignId(
		std::make_unique<PhysicalModel::PEC>(
			PhysicalModel::Id(),
			"Material PEC"
		)
	);

	const auto& physicalModelIt = physicalModelsGroup.addAndAssignId(
		std::make_unique<PhysicalModel::Bound>(PhysicalModel::Id(), PhysicalModel::Bound::Type::pml)
	);

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

	coordinatesGroup.addAndAssignId(
		std::make_unique<Geometry::CoordR3>(
			Geometry::Coordinate::Id(),
			Math::CVecR3(5.0, 5.0, 5.0)
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

	const Geometry::CoordR3* coordinatesArgumentList2[1] = { coordinatesGroup.getId(Geometry::Coordinate::Id(2)) };
	elementsGroup.addAndAssignId(
		std::make_unique<Geometry::NodR>(
			Geometry::Element::Id(),
			coordinatesArgumentList2
		)
	);

	// Boundaries
	const Geometry::CoordR3* coordinatesArgumentBoundaryList[1] = { coordinatesGroup.getId(Geometry::Coordinate::Id(3)) };
	elementsGroup.addAndAssignId(
		std::make_unique<Geometry::NodR>(
			Geometry::ElemId(),
			coordinatesArgumentBoundaryList,
			nullptr,
			physicalModelIt->get()
		)
	);

	const Model::Model model = Model::Model(
		Geometry::Mesh::Unstructured(coordinatesGroup, elementsGroup),
		physicalModelsGroup
	);

	problemDescription.model = model;

	ASSERT_FALSE(problemDescription.model.physicalModels.empty());
	EXPECT_EQ("Material PEC", problemDescription.model.physicalModels.get()[0]->getName());
	EXPECT_EQ("PML_Bound", problemDescription.model.physicalModels.get()[1]->getName());

	EXPECT_FALSE(problemDescription.model.unstructuredMesh.coords().empty());
	EXPECT_EQ(
		Math::CVecR3(1.0, 2.0, 3.0),
		(problemDescription.model.unstructuredMesh.coords().get()[1])->pos()
	);

	const auto& element = problemDescription.model.unstructuredMesh.elems().getId(Geometry::ElemId(3));

	EXPECT_EQ(
		element->getMatId(),
		problemDescription.model.physicalModels.get()[1]->getId()
	);

	EXPECT_EQ(
		element->getCoordinates()[0]->pos(),
		Math::CVecR3(5.0, 5.0, 5.0)
	);
}

TEST(ProblemDescriptionTest, CanInitializeOutputRequests) {
	ProblemDescription problemDescription = ProblemDescription();

	OutputRequest::OutputRequest::Target target = OutputRequest::OutputRequest::Target();	
	const Geometry::CoordR3* coords[1] = {new Geometry::CoordR3(Geometry::CoordId(), Math::CVecR3(1.0, 2.0, 3.0))};
	const Geometry::NodR node = Geometry::NodR(
		Geometry::Element::Id(),
		coords		
	);
	target.push_back(&node);

	OutputRequestGroup probes = OutputRequestGroup();
	probes.addAndAssignId(
		std::make_unique<OutputRequest::OnPoint>(
			OutputRequest::OutputRequest::Type::electric,
			OutputRequest::Domain(),
			"My electric field point probe",
			target
		)
	);

	problemDescription.outputRequests = probes;

	EXPECT_EQ(1, problemDescription.outputRequests.size());
	EXPECT_EQ(
		"My electric field point probe",
		problemDescription.outputRequests.get()[0]->getName()
	);
	EXPECT_EQ(
		OutputRequest::OutputRequest::Type::electric,
		problemDescription.outputRequests.get()[0]->getType()
	);
	EXPECT_EQ(
		1,
		(problemDescription.outputRequests.get()[0]->getTarget()).size()
	);

	auto recoveredNode = problemDescription.outputRequests.get()[0]->getTarget().at(0)->castTo<Geometry::NodR>();

	EXPECT_EQ(
		Math::CVecR3(1.0, 2.0, 3.0),
		recoveredNode->getV(0)->pos()
	);
}

TEST(ProblemDescriptionTest, CanCopyConstructor) {
	ProblemDescription problemDescription = ProblemDescription();

	// Create mesh
	Geometry::CoordR3Group coordinatesGroup = Geometry::CoordR3Group();
	coordinatesGroup.addAndAssignId(
		std::make_unique<Geometry::CoordR3>(
			Geometry::Coordinate::Id(1),
			Math::CVecR3(0.0, 0.0, 0.0)
			)
	);
	coordinatesGroup.addAndAssignId(
		std::make_unique<Geometry::CoordR3>(
			Geometry::Coordinate::Id(2),
			Math::CVecR3(1.0, 2.0, 3.0)
			)
	);

	const Geometry::CoordR3* coordinatesArgumentList[1] = { coordinatesGroup.getId(Geometry::Coordinate::Id(1)) };
	Geometry::ElemRGroup elementsGroup = Geometry::ElemRGroup();
	elementsGroup.addAndAssignId(
		std::make_unique<Geometry::NodR>(
			Geometry::Element::Id(1),
			coordinatesArgumentList
		)
	);

	const Geometry::CoordR3* coordinatesArgumentList2[1] = { coordinatesGroup.getId(Geometry::Coordinate::Id(2)) };
	elementsGroup.addAndAssignId(
		std::make_unique<Geometry::NodR>(
			Geometry::Element::Id(2),
			coordinatesArgumentList2
		)
	);

	// This mesh has 2 coordinates and 2 elements (Node)
	Geometry::Mesh::Unstructured unstructuredMesh = Geometry::Mesh::Unstructured(coordinatesGroup, elementsGroup);

	// Create source
	Source::Group<> sources = Source::Group<>();

	Math::CVecR3 dir(1.0, 0.0, 0.0);
	Math::CVecR3 pol(0.0, 0.0, 1.0);

	Source::PlaneWave::Target sourceTarget;
	sourceTarget.push_back(elementsGroup.getId(Geometry::Element::Id(1)));

	Source::PlaneWave planewave = Source::PlaneWave(
		std::make_unique<Source::Magnitude::Magnitude>(
			new SEMBA::Math::Function::Gaussian(0.5, 0.0, 1.0)
		),
		sourceTarget,
		dir,
		pol
	);

	sources.addAndAssignId(std::make_unique<Source::PlaneWave>(planewave));
	
	// Create OutputRequest
	
	OutputRequest::OutputRequest::Target target;
	target.push_back(elementsGroup.getId(Geometry::Element::Id(2)));

	OutputRequestGroup probes = OutputRequestGroup();
	probes.addAndAssignId(
		std::make_unique<OutputRequest::OnPoint>(
			OutputRequest::OutputRequest::Type::electric,
			OutputRequest::Domain(),
			"My electric field point probe",
			target
		)
	);
	
	// Create Model
	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.addAndAssignId(
		std::make_unique<PhysicalModel::PEC>(
			PhysicalModel::Id(),
			"Material PEC"
			)
	);

	const Model::Model model = Model::Model(
		unstructuredMesh,
		physicalModelsGroup
	);

	// Assign to ProblemDescription
	problemDescription.sources = sources;
	problemDescription.outputRequests = probes;
	problemDescription.model = model;

	// Call copy constructor and check
	ProblemDescription copy(problemDescription);

	auto originalCoordinatePointer = problemDescription.outputRequests.get().front()->getTarget().front()->castTo<Geometry::NodR>()->getV(0);
	auto newCoordinatePointer = copy.outputRequests.get().front()->getTarget().front()->castTo<Geometry::NodR>()->getV(0);

	EXPECT_NE(originalCoordinatePointer, newCoordinatePointer);
	EXPECT_EQ(*originalCoordinatePointer, *newCoordinatePointer);

	EXPECT_NE(newCoordinatePointer, coordinatesGroup.getId(Geometry::CoordId(2)));
	EXPECT_EQ(*newCoordinatePointer, *coordinatesGroup.getId(Geometry::CoordId(2)));
}
