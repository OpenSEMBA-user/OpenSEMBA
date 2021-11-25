#include "gtest/gtest.h"

#include "physicalModel/Group.h"
#include "physicalModel/Bound.h"
#include "physicalModel/volume/Classic.h"
#include "physicalModel/multiport/Predefined.h"

using namespace SEMBA;
using namespace PhysicalModel;

class PhysicalModelGroupTest : public ::testing::Test {
public:

protected:
    auto buildPMGroup()
    {
        PMGroup pm;
        pm.add(std::make_unique<Volume::Classic>(Id(1), "Classic", 1.0, 1.0, 0.0, 0.0));
        pm.add(std::make_unique<Bound>(Id(2), Bound::Type::pec));
        return pm;
    }
};

TEST_F(PhysicalModelGroupTest, copy_ctor) 
{
    PMGroup copied(buildPMGroup());

    EXPECT_EQ(2, copied.size());
}

TEST_F(PhysicalModelGroupTest, copy_assignment)
{
    PMGroup copied = buildPMGroup();
    
    EXPECT_EQ(2, copied.size());
}

TEST_F(PhysicalModelGroupTest, move_assignment)
{
    PMGroup original = buildPMGroup();
    PMGroup moved = std::move(original);

    EXPECT_EQ(0, original.size());
    EXPECT_EQ(2, moved.size());
}

TEST_F(PhysicalModelGroupTest, bound) 
{
    PMGroup bounds;
    bounds.add(std::make_unique<Bound>( Id(1), Bound::Type::pec ));
    bounds.add(std::make_unique<Bound>( Id(2), Bound::Type::pmc ));

    const Bound* bound = bounds.getOf<Bound>()[0];
    EXPECT_TRUE(bound->getType() == Bound::Type::pec);
}

TEST_F(PhysicalModelGroupTest, copy_and_getOf)
{
    PMGroup original = buildPMGroup();
    original.addAndAssignId(std::make_unique<Multiport::Predefined>(
        Id{ 0 }, "Short", Multiport::Multiport::Type::shortCircuit)
    );

    EXPECT_EQ(1, original.sizeOf<Volume::Classic>());
    EXPECT_EQ(1, original.sizeOf<Multiport::Predefined>());

    PMGroup copied(original);
    EXPECT_EQ(1, copied.sizeOf<Volume::Classic>());
    EXPECT_EQ(1, copied.sizeOf<Multiport::Predefined>());
}