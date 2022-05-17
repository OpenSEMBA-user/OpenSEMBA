#pragma once

#include "Boundary.h"

using namespace SEMBA::Boundary;

Boundary::Boundary(const std::string lower[3], const std::string upper[3]) {
	for (int i = 0; i < 3; i++) {
		this->lower[i] = lower[i];
		this->upper[i] = upper[i];
	}
}
