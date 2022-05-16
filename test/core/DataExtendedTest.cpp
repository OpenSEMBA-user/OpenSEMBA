#pragma once

#include "gtest/gtest.h"

#include "DataExtended.h"

using namespace SEMBA;

TEST(DataExtendedTest, CanCreate) {
	DataExtended dataExtended = DataExtended();

	EXPECT_NE(&dataExtended, nullptr);
	EXPECT_EQ(dataExtended.boundary, nullptr);
}

TEST(DataExtendedTest, CanInitializeBoundary) {
	Boundary::Boundary boundary = Boundary::Boundary();

	DataExtended dataExtended = DataExtended(&boundary);

	EXPECT_EQ(dataExtended.boundary, &boundary);
	EXPECT_EQ(dataExtended.boundary->lower[0], "PML");
}
