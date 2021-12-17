#pragma once

#include <chrono>

namespace SEMBA {
namespace FileSystem {

class Chronometer {
public:
	
	void start() {
		timeStart_ = std::chrono::high_resolution_clock::now();
	}
	void stop() {
		timeEnd_ = std::chrono::high_resolution_clock::now();
	}

	void print() {
		using namespace std::chrono;
		duration<double> timeElapsed = duration_cast<duration<double>>(timeEnd_ - timeStart_);
		std::size_t s = duration_cast<seconds>(timeElapsed).count() % 60;
		std::size_t m = duration_cast<minutes>(timeElapsed).count() % 60;
		std::size_t h = duration_cast<hours>(timeElapsed).count();
		std::cout << " " << "Time elapsed: ";
		if (h > 99) {
			std::cout << "more than 100 h";
		}
		else {
			std::cout << h << " h, " << m << " m, " << s << " s";
		}
		std::cout << std::endl;
	}

private:
	std::chrono::high_resolution_clock::time_point timeStart_;
	std::chrono::high_resolution_clock::time_point timeEnd_;
};

}
}