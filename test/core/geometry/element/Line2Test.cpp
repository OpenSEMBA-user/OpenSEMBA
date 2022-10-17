#include "gtest/gtest.h"

#include "geometry/element/Line2.h"

using namespace SEMBA;

TEST(Line2Test, CanCreate) {
	Geometry::CoordR3 vertexLeft{ Geometry::CoordId(), Math::CVecR3({0.0, 0.0, 0.0}) };
	Geometry::CoordR3 vertexRight{ Geometry::CoordId(), Math::CVecR3({1.0, 0.0, 0.0}) };

	Geometry::Layer::Layer lay{Geometry::LayerId(), "My layer"};
	Geometry::Element::Model model{MatId()};

	Geometry::LinR2 lin{ Geometry::ElemId(), {&vertexLeft, &vertexRight}, &lay, &model };

	auto newLines = lin.splitByMiddle();

	EXPECT_EQ(
		*(newLines[0]->getVertices()[0]),
		*lin.getVertices()[0]
	);
	EXPECT_EQ(
		*(newLines[0]->getVertices()[1]),
		Geometry::CoordR3(Geometry::CoordId(), Math::CVecR3(0.5, 0.0, 0.0))
	);
	EXPECT_EQ(&lay, newLines[0]->getLayer());
	EXPECT_EQ(&model, newLines[0]->getModel());

	EXPECT_EQ(
		*(newLines[1]->getVertices()[0]),
		Geometry::CoordR3(Geometry::CoordId(), Math::CVecR3(0.5, 0.0, 0.0))
	);
	EXPECT_EQ(
		*(newLines[1]->getVertices()[1]),
		*lin.getVertices()[1]
	);
	EXPECT_EQ(&lay, newLines[1]->getLayer());
	EXPECT_EQ(&model, newLines[1]->getModel());

	EXPECT_EQ(newLines[0]->getLayer()->getName(), lay.getName());
}
