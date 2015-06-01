/*
 * UGRMesherIntersectionsGeneratorTest.h
 *
 *  Created on: May 22, 2015
 *      Author: Miguel D. Ruiz-Cabello N.
 */

#include "../../../../../mesher/fdtd/meshConf/IntersectionsGenerator.h"

#include "gtest/gtest.h"

class MesherIntersectionsGeneratorTest : public ::testing::Test {
public:
    void init(const CVecR3 &min_,const CVecR3 &max_,
              const CVecR3 &step_, const vector<CoordR3*> coords) {
        //create grid
        grid        = new Grid3(BoxR3(min_, max_), step_);
        //create uMesh
        cG_ = CoordR3Group(coords);
        vector<ElemR*> elems;
        CoordinateId vId[3] = {CoordinateId(1), CoordinateId(2), CoordinateId(3)};
        elems.push_back(new Tri3(cG_, ElementId(1), vId, LayerId(0), MatId(1)));
        eG_ = ElemRGroup(elems);
        uMesh = new MeshUnstructured (cG_, eG_);
    };

    const Grid3*   grid;
    MeshUnstructured*  uMesh;

private:
    CoordR3Group   cG_;
    ElemRGroup     eG_;

};
