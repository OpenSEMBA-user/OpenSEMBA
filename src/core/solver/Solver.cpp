

#include "Solver.h"

namespace SEMBA {
namespace Solver {

Solver::Solver() {
}

Solver::~Solver() {
}

double Solver::storeCPUTime() const {
#ifdef _WIN32
    LPFILETIME tim = nullptr;
    GetSystemTimeAsFileTime(tim);
    return tim->dwLowDateTime;

#else
    timeval tim;
    gettimeofday(&tim, nullptr);
    return tim.tv_sec + (tim.tv_usec/1000000.0);
#endif
}

void Solver::printTimeProfilingInfo(double tSum,
                                    double tRunning,
                                    const std::size_t tStep,
                                    const std::size_t NtSteps) const {
    if (tStep % printStep == 0) {
        // Prints information on how much took to complete the current
        // set of time steps.
        std::cout << "Step " << tStep << "/" << NtSteps << " || ";
        // Makes prediction on how much will take to complete.
        double tCPUFinal = 0.0;
        tRunning += tSum;
        tCPUFinal = tRunning * NtSteps / tStep;
        printTime(tCPUFinal - tRunning);
        std::cout<< " remaning." << std::endl;
    }
}

void
Solver::printTime(const double originalSeconds) const {
    int hours = 0;
    int minutes = 0;
    int secondsLeft = 0;
    hours = int(originalSeconds) / 3600;
    secondsLeft = int(originalSeconds) - hours * 3600;
    minutes = int (secondsLeft) / 60;
    secondsLeft = secondsLeft - minutes * 60;
    if (hours > 0) {
        std::cout<< hours << "h ";
    } if (minutes > 0) {
        std::cout<< minutes << "m ";
    }
    std::cout<< secondsLeft << "s";
}

} /* namespace Solver */
} /* namespace SEMBA */
