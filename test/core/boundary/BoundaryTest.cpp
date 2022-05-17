#pragma once

#include "gtest/gtest.h"

#include "core/boundary/Boundary.h"

using namespace SEMBA::Boundary;

TEST(BoundaryTest, CanCreate) {
	Boundary boundary = Boundary::Boundary();

	EXPECT_NE(&boundary, nullptr);

	for (int i = 0; i < boundary.lower->size(); i++) {
		EXPECT_EQ(boundary.lower[i], "PML");
		EXPECT_EQ(boundary.upper[i], "PML");
	}
}

TEST(BoundaryTest, CanCreateWithDifferentType) {

	const std::string lower[3] = { "PML", "PEC", "PMC" };
	const std::string upper[3] = { "MUR1", "MUR2", "Periodic" };
	Boundary boundary = Boundary::Boundary(lower, upper);


	for (int i = 0; i < 3; i++) {
		EXPECT_EQ(boundary.lower[i], lower[i]);
		EXPECT_EQ(boundary.upper[i], upper[i]);
	}
}