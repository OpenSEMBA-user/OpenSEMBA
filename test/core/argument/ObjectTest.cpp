// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

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
