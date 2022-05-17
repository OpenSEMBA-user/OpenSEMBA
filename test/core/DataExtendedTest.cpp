#pragma once

#include "gtest/gtest.h"
#include "geometry/Grid.h"
#include "DataExtended.h"

using namespace SEMBA;

TEST(DataExtendedTest, CanCreate) {
	DataExtended dataExtended = DataExtended();

	EXPECT_NE(&dataExtended, nullptr);
	EXPECT_EQ(dataExtended.boundary, nullptr);
}

TEST(DataExtendedTest, CanInitializeBoundary) {
	Boundary::Boundary boundary = Boundary::Boundary();
	DataExtended dataExtended = DataExtended();
	dataExtended.boundary = &boundary;

	EXPECT_EQ(dataExtended.boundary, &boundary);
	EXPECT_EQ(dataExtended.boundary->lower[0], "PML");
}

TEST(DataExtendedTest, CanInitializeGrid) {
	DataExtended dataExtended = DataExtended();
	dataExtended.grid3 = new Geometry::Grid3(
		Geometry::BoxR3(
			Math::CVecR3(0.0, 0.0, 0.0),
			Math::CVecR3(1.732050807568877, 1.732050807568877, 1.732050807568877)
		),
		Math::CVecR3(0.5, 0.5, 0.5)
	);

	EXPECT_EQ(dataExtended.boundary, nullptr);
	EXPECT_EQ(dataExtended.grid3->getNumCells(), Math::CVecR3(4, 4, 4)); // TODO: Check why 4
}
