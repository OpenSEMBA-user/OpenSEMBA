

#pragma once

#include <iostream>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "filesystem/Project.h"

namespace SEMBA {
namespace Solver {

class Solver {
public:
    Solver();
    virtual ~Solver();

    virtual bool run() = 0;
    virtual bool canRun() const = 0;
    virtual bool isRunSimulation() const = 0;

    virtual FileSystem::Project getFolder() const = 0;
protected:
    static const std::size_t printStep = 100;
    void printTime(const double originalSeconds) const;
    double storeCPUTime() const;
    void printTimeProfilingInfo(double tSum,
                                double tRunning,
                                const std::size_t tStep,
                                const std::size_t Ntsteps) const;
    //   void
    //   processStopRequest();
    //  Int
    //   kbhit();
    //  void
    //   nonblock(Int state);
};

} /* namespace Solver */
} /* namespace SEMBA */

