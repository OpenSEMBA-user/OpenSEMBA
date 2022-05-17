#pragma once

#include "string"

namespace SEMBA {
namespace Boundary {
	class Boundary {
	public:
		std::string lower[3] = {"PML", "PML", "PML"};
		std::string upper[3] = { "PML", "PML", "PML" };

		Boundary() = default;
		Boundary(const std::string lower[3], const std::string upper[3]);
	};
}
}