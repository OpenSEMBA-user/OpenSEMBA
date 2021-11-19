#pragma once

#include "gtest/gtest.h"

#include "geometry/element/Tetrahedron4.h"
#include "geometry/mesh/Mesh.h"
#include "geometry/element/Group.h"

using namespace std;

using namespace SEMBA;
using namespace Geometry;
using namespace Math;

class MeshTest {
public:
    void SetUp() {
        CoordId coordId(1);
        cG_.add(std::make_unique<CoordR3>(coordId++, CVecR3(0.0, 0.0, 0.0)));
        cG_.add(std::make_unique<CoordR3>(coordId++, CVecR3(0.0, 0.0, 1.0)));
        cG_.add(std::make_unique<CoordR3>(coordId++, CVecR3(0.0, 1.0, 0.0)));
        cG_.add(std::make_unique<CoordR3>(coordId++, CVecR3(1.0, 0.0, 0.0)));
        
        vector<ElemR*> elems;
        const CoordR3* vTet[4] = {
                cG_.getId(CoordId(1))->get(),
                cG_.getId(CoordId(2))->get(),
                cG_.getId(CoordId(3))->get(),
                cG_.getId(CoordId(4))->get()
        };
        const CoordR3* vTri[3] = {
                cG_.getId(CoordId(2))->get(),
                cG_.getId(CoordId(1))->get(),
                cG_.getId(CoordId(3))->get()
        };
        elems.push_back(new Tet4(ElemId(1), vTet));
        elems.push_back(new Tri3(ElemId(2), vTri));
        eG_ = ElemRGroup(elems);
        lG_ = Layer::Group<>();
    }

protected:
    CoordR3Group cG_;
    ElemRGroup eG_;
    Layer::Group<> lG_;

};

