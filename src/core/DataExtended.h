#pragma once

#include "Data.h"
#include "geometry/Grid.h"
#include "physicalModel/Bound.h"

namespace SEMBA {

    class DataExtended : public Data {
    public:
        std::vector<std::pair<PhysicalModel::Bound, PhysicalModel::Bound>> boundary;
        std::unique_ptr<Geometry::Grid3> grid3;

        DataExtended() = default;
        DataExtended(
            const std::vector<std::pair<PhysicalModel::Bound, PhysicalModel::Bound>>& boundary,
            const Geometry::Grid3& grid3
        );
    };

}
