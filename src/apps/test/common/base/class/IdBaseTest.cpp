#include "gtest/gtest.h"
#include "base/class/IdBase.h"

CREATE_ID(PatataId);

class BaseIdBaseTest : public ::testing::Test {

};

TEST_F(BaseIdBaseTest, PostIncrement) {
    PatataId id(1);
    EXPECT_EQ(PatataId(1), id++);
}

TEST_F(BaseIdBaseTest, Preincrement) {
    PatataId id(1);
    EXPECT_EQ(PatataId(2), ++id);
}
