

#include "ObjectTest.h"

using namespace std;
using namespace SEMBA;


TEST_F(ObjectTest, asDictionary) {
    Argument::Object dict;
    EXPECT_NO_THROW(dict("key1").setString("val1"));
    EXPECT_NO_THROW(dict("key2").setString("val2"));
    EXPECT_EQ("val1", dict("key1").getString());
    EXPECT_ANY_THROW(dict("key1").getBool());
}

TEST_F(ObjectTest, asSet) {
    Argument::Object obj;
    obj.set(1, "patata");
    tuple<int,string> data = obj.get<int,string>();

    EXPECT_EQ(1, get<0>(data));
    EXPECT_EQ("patata", get<1>(data));
}
