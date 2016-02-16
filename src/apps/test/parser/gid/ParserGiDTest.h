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
#ifndef PARSERGIDTEST_H_
#define PARSERGIDTEST_H_

#include "gtest/gtest.h"
#include "parser/gid/Parser.h"

class ParserGiDTest : public ::testing::Test {

protected:

   ParserGiDTest() {
   }

   virtual ~ParserGiDTest() {
   }

   SmbData* newSmb(const string project) {
      cout << "Project: " << project << endl;
      const string testFolder("./projects/test/");
      ParserGiD parser(testFolder+project+".gid/"+project+".smb");
      EXPECT_TRUE(parser.canOpen());
      SmbData* res = parser.read();
      EXPECT_TRUE(res != NULL);
      if (res != NULL) {
          EXPECT_TRUE(res->check());
      }
      return res;
   }
};

#endif
