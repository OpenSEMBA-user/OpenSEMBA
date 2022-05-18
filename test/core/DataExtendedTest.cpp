#pragma once

#include "gtest/gtest.h"
#include "geometry/Grid.h"
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
}

TEST(DataExtendedTest, CanInitializeGrid) {
	DataExtended dataExtended = DataExtended();
	Geometry::Grid3 grid3 = Geometry::Grid3(
		Geometry::BoxR3(
			Math::CVecR3(0.0, 0.0, 0.0),
			Math::CVecR3(1.732050807568877, 1.732050807568877, 1.732050807568877)
		),
		Math::CVecR3(0.5, 0.5, 0.5)
	);

	dataExtended.grid3 = std::make_unique<Geometry::Grid3>(grid3);

	EXPECT_TRUE(dataExtended.boundary.empty());
	EXPECT_EQ(dataExtended.grid3->getNumCells(), Math::CVecR3(4, 4, 4)); // TODO: Check why 4
}
