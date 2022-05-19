#pragma once

#include "gtest/gtest.h"
#include "geometry/Grid.h"
#include "source/PlaneWave.h"
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
	EXPECT_EQ(dataExtended.sources, nullptr);
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
	EXPECT_EQ(dataExtended.sources, nullptr);
}

TEST(DataExtendedTest, CanInitializeSources) {
	DataExtended dataExtended = DataExtended();
	
	Source::Group<>* sources = new Source::Group<>();

	Math::CVecR3 dir(1.0, 0.0, 0.0);
	Math::CVecR3 pol(0.0, 0.0, 1.0);
	Source::PlaneWave* planewavePointer = new Source::PlaneWave(nullptr, Geometry::ElemRGroup(), dir, pol);

	sources->add(planewavePointer);

	dataExtended.sources = sources;

	EXPECT_TRUE(dataExtended.boundary.empty());
	EXPECT_EQ(dataExtended.grids, Geometry::Grid3());

	EXPECT_EQ(dataExtended.sources, sources);
	EXPECT_EQ(dataExtended.sources->size(), 1);
	EXPECT_EQ(dataExtended.sources->get(0), planewavePointer);
}
