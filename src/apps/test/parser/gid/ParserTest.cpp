//// OpenSEMBA
//// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
////                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
////                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
////                    Daniel Mateos Romero            (damarro@semba.guru)
////
//// This file is part of OpenSEMBA.
////
//// OpenSEMBA is free software: you can redistribute it and/or modify it under
//// the terms of the GNU Lesser General Public License as published by the Free
//// Software Foundation, either version 3 of the License, or (at your option)
//// any later version.
////
//// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
//// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
//// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
//// details.
////
//// You should have received a copy of the GNU Lesser General Public License
//// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
//#include "gtest/gtest.h"
//
//#include "parser/gid/Parser.h"
//
//#include "geometry/element/Line2.h"
//#include "geometry/element/Triangle3.h"
//#include "geometry/element/Tetrahedron4.h"
//
//using namespace std;
//using namespace SEMBA;
//using namespace Parser;
//
//class ParserGiDParserTest : public ::testing::Test {
//protected:
//    static constexpr Math::Real tol_ = 1e-4;
//    GiD::ProblemSize pSize_;
//
//    Data* newSmb(const std::string project) {
//        const string testFolder("./testData/");
//            const string testFile = testFolder +
//                    project + ".gid/" + project + ".dat";
//        #ifndef _WIN32
//            string fullPath((const char*) realpath(testFile.c_str(), nullptr));
//        #else
//            string fullPath = testFile;
//        #endif
//            GiD::Parser parser(fullPath);
//            EXPECT_TRUE(parser.canOpen()) << "Can't open: " << fullPath;
//            Data* res = parser.read();
//            EXPECT_TRUE(res != nullptr);
//            if (res != nullptr) {
//                EXPECT_TRUE(res->check());
//            }
//
//            pSize_ = parser.getProblemSize();
//
//            return res;
//    }
//
//    static void checkMatchBetweenProblemSizesAndReadSizes(
//            const GiD::ProblemSize& pSize,
//            const Geometry::Mesh::Geometric* mesh) {
//
//        using namespace SEMBA::Geometry;
//
//        // NOTE: Not all sizes will match after parsing.
//        //       Some elements are added by others (grid, outRq, planewave...)
//        EXPECT_EQ(pSize.tri3, mesh->elems().sizeOf<Tri3>());
//        EXPECT_EQ(pSize.tet4, mesh->elems().sizeOf<Tet4>());
//        EXPECT_EQ(pSize.lin2, mesh->elems().sizeOf<LinR2>());
//    }
//};
//
//
//TEST_F(ParserGiDParserTest, Cartesian) {
//    Data* smb;
//    EXPECT_NO_THROW(smb = newSmb("cartesian"));
//    if (smb == nullptr) {
//        return;
//    }
//
//    const Geometry::Mesh::Geometric* mesh;
//    EXPECT_NO_THROW(mesh = smb->mesh->castTo<Geometry::Mesh::Geometric>());
//    checkMatchBetweenProblemSizesAndReadSizes(pSize_, mesh);
//
//    const Geometry::CoordR3* coord;
//    EXPECT_NO_THROW(coord = mesh->coords().getId(Geometry::CoordId(1)));
//    EXPECT_NEAR(Math::CVecR3(-1.8e-3)(0), coord->pos()(0), tol_);
//    EXPECT_NEAR(Math::CVecR3(-1.8e-3)(1), coord->pos()(1), tol_);
//    EXPECT_NEAR(Math::CVecR3(-1.8e-3)(2), coord->pos()(2), tol_);
//
//    Solver::Settings settings;
//    EXPECT_NO_THROW(settings = smb->solver->getSettings());
//
//    Geometry::Grid3 grid;
//    EXPECT_NO_THROW(grid =
//            smb->mesh->castTo<Geometry::Mesh::Geometric>()->grid());
//    EXPECT_EQ(Math::CVecR3(-2.1e-3), grid.getFullDomainBoundingBox().getMin());
//
//    EXPECT_NO_THROW(delete smb);
//}
//
//TEST_F(ParserGiDParserTest, Sphere) {
//    Data* smb;
//    EXPECT_NO_THROW(smb = newSmb("sphere"));
//    if (smb == nullptr) {
//        return;
//    }
//
//    const Geometry::Mesh::Geometric* mesh;
//    EXPECT_NO_THROW(mesh = smb->mesh->castTo<Geometry::Mesh::Geometric>());
//    checkMatchBetweenProblemSizesAndReadSizes(pSize_, mesh);
//
//    EXPECT_EQ(smb->outputRequests->getOf<OutRqNode>().size(), 3);
//    EXPECT_EQ(smb->outputRequests->getOf<OutRqSurface>().size(), 1);
//    EXPECT_EQ(smb->outputRequests->getOf<OutRqVolume>().size(), 1);
//    EXPECT_EQ(smb->sources->size(), 1);
//
//
//    const Geometry::CoordR3* coord;
//    EXPECT_NO_THROW(coord =mesh->coords().getId(Geometry::CoordId(1)));
//    EXPECT_NEAR(Math::CVecR3(-1.8e-3)(0), coord->pos()(0), tol_);
//    EXPECT_NEAR(Math::CVecR3(-1.8e-3)(1), coord->pos()(1), tol_);
//    EXPECT_NEAR(Math::CVecR3(-1.8e-3)(2), coord->pos()(2), tol_);
//
//    Solver::Settings settings;
//    EXPECT_NO_THROW(settings = smb->solver->getSettings());
//
//    Geometry::Grid3 grid;
//    EXPECT_NO_THROW(grid =
//            smb->mesh->castTo<Geometry::Mesh::Geometric>()->grid());
//    EXPECT_EQ(Math::CVecR3(-2.1e-3), grid.getFullDomainBoundingBox().getMin());
//
//    EXPECT_NO_THROW(delete smb);
//}
//
//TEST_F(ParserGiDParserTest, DMCWF) {
//    Data* smb;
//    EXPECT_NO_THROW(smb = newSmb("dmcwf"));
//    if (smb == nullptr) {
//        return;
//    }
//    EXPECT_NO_THROW(delete smb);
//}
