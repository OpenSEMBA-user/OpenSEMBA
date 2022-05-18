#pragma once

#include "Data.h"
#include "boundary/Boundary.h"
#include "geometry/Grid.h"

namespace SEMBA {

    class DataExtended : public Data {
    public:
        Boundary::Boundary* boundary;
        std::unique_ptr<Geometry::Grid3> grid3;

        DataExtended();
        DataExtended(
            Boundary::Boundary* boundary, 
            const Geometry::Grid3& grid3
        );
    };

}
