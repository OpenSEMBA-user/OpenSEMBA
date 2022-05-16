#pragma once

#include "Data.h"
#include "boundary/Boundary.h"

namespace SEMBA {

    class DataExtended : public Data {
    public:
        Boundary::Boundary* boundary;

        DataExtended();
        DataExtended(Boundary::Boundary* boundary);
    };

}
