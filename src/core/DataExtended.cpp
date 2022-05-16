#pragma once

#include "DataExtended.h"

namespace SEMBA {

DataExtended::DataExtended(): Data() {
	boundary = nullptr;
}

DataExtended::DataExtended(Boundary::Boundary* boundary) :
	Data(),
	boundary(boundary)
{}
}
