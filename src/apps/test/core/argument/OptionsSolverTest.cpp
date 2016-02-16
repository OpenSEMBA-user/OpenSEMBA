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
#include "gtest/gtest.h"

//class OptionsSolverTest :
//        public ::testing::Test,
//        public ::testing::WithParamInterface<const char*> {
//
//};
//
//TEST_P(OptionsSolverTest, conversion){
//    string str = GetParam();
//    string pre = "EXEC -i ./projects/test/sphere.gid/sphere.smb";
//    Arguments inputArg(pre + str);
//    OptionsSolver options;
//    options.set(inputArg);
//    Arguments convertedArg(pre + options.toArgsStr());
//    EXPECT_TRUE(convertedArg.contains(inputArg));
//}
//
//INSTANTIATE_TEST_CASE_P(
//      goodArguments,
//      OptionsSolverTest,
//      ::testing::Values(
//            " -mtln -groundwires",
//            " -attw 0.1 -pmlalpha 0.2 0.1",
//            " -ade -taparrabos",
//            " -pmlcorr 0.1 14",
//            " -inductance ledfelt"
//            ));
