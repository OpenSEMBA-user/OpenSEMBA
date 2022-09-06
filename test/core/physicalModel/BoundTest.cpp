#include "gtest/gtest.h"
#include <array>

#include "physicalModel/Bound.h"

using namespace SEMBA;
using namespace PhysicalModel;

TEST(PhysicalModelBoundTest, TestGetTypeName) {
	using EntryType = std::pair<Bound, std::string>;

	std::array<EntryType, 6> list = {
		EntryType(Bound(Id(), Bound::Type::pec), "PEC"),
		EntryType(Bound(Id(), Bound::Type::pmc), "PMC"),
		EntryType(Bound(Id(), Bound::Type::pml), "PML"),
		EntryType(Bound(Id(), Bound::Type::periodic), "Periodic"),
		EntryType(Bound(Id(), Bound::Type::mur1), "MUR1"),
		EntryType(Bound(Id(), Bound::Type::mur2), "MUR2")
	};

	for (const EntryType& item : list) {
		EXPECT_EQ(item.first.getTypeName(), item.second);
		EXPECT_EQ(item.first.getName(), item.second + "_Bound");
	}

	try {
		Bound(Id(), Bound::Type::sma).getTypeName();
		FAIL() << "No exception was thrown";
	}
	catch (const std::logic_error& exception) {
		EXPECT_STREQ(exception.what(), "Unrecognized value for Bound type name");
	}
}
