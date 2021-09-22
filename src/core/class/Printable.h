

#pragma once

#include <sstream>
#include <iostream>

namespace SEMBA {
namespace Class {

class Printable {
public:
    Printable() {}
    virtual ~Printable() {}

    virtual void printInfo() const = 0;

	std::string str() const {
		std::stringstream buffer;
		std::streambuf * old = std::cout.rdbuf(buffer.rdbuf());

		this->printInfo();

		std::cout.rdbuf(old);
		return buffer.str();
	}
};

} /* namespace Class */
} /* namespace SEMBA */

